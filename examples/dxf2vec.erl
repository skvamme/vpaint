%% Copyright (C) 2009 by S Kvamme
%% All rights reserved.
%% The copyright holder hereby grants the rights of usage, distribution
%% and modification of this software to everyone and for any purpose, as
%% long as this license and the copyright notice above are preserved and
%% not modified. There is no warranty for this software.

%% dxf2vec:start(["proctor_heel_plug1.dxf","0"]).


      % <face
      %   id="11"
      %   cycles="[3+] [6+] [4+] [5+] [7+] [8+] [9+] [0+] [1+] [2+]"
      %   color="rgba(153,102,51,1)"/>

-module(dxf2vec).
-author(skvamme).
-compile(export_all).
-import(lists, [reverse/1]).
-import(ets,[lookup/2,insert/2]).
-import(math,[pi/0,pow/2,sqrt/1,atan2/2]).
-define(DOUBLE, 64/little-float).
-define(DWORD, 32/unsigned-little-integer).
-define(WORD, 16/unsigned-little-integer).
-define(BYTE, 8/unsigned-little-integer).

%% 080612	First version												S Kvamme
%% 081213	Bug in setting of Color fixed								S Kvamme
%% 081213	Bug in sorting fixed. Changed to sort Thicknes (was Elevation).	S Kvamme
%% 090909	Added support for ASCII DXF									S Kvamme
%% 100117	Bug in entity color fixed (QCAD)							S Kvamme
%% 100619	Changed output format from erlang to Javascript				S Kvamme
%% 201026	Changed output format from Javascript to VPaint vec			S Kvamme

%****************************************************************************
% Read dxf file and otput corresponding vec source for VPaint graphics
%****************************************************************************
start(Args) ->
	DXF = hd(Args),
	Layerarray = tl(Args),
	Etable = ets:new(entity,[duplicate_bag,private]), % Store all the entities in this table
	Ttable = ets:new(tmp,[set,private]), % Store temporary group values here
	put(edge,0), % holds edge numbering
	put(vertex,0), % holds vertex numbering, vill be an ets with x and y
	lists:foreach( fun(Layer) -> 
		case read_dxf_tag(DXF) of
			error -> io:format("//Cannot read file: ~p~n",[DXF]);
			ascii ->
				{ok, F} = file:open(DXF, read),
				find_header_ascii(F),
				{{_X1,[]},{_Y1,[]},{_X2,[]},{_Y2,[]}} = limits_ascii(F),
				%io:format("Limits: ~p~n",[{X1,Y1,X2,Y2}]),
				print_header(),
				find_entities_ascii(F),
				io:get_line(F, ''), % get rid of "  0"
				entities_ascii(F,Etable,Layer,trim(io:get_line(F, '')));
			bin -> 
				{ok, B} = file:read_file(DXF),
				{_,B1} = split_binary(B, 22),
				B2 = find_header(B1),
				%limits(B2),
				print_header(),
				B3 = find_entities(B2),
				entities(Etable,B3,Layer)
		end, %io:format("About to print_entities~n",[]),
			prints_entities(Etable,Ttable) end,Layerarray),
		io:format("</objects>~n</layer>~n"),
		io:format("</vec>~n").

%****************************************************************************************
% Function find_entities_ascii(F1), 
%****************************************************************************************
find_entities_ascii(F) -> fea(F,"").

fea(_F,"ENTITIES") -> ok;
fea(_F,eof) -> io:format("Didn't find entities section~n",[]),erlang:halt();
fea(F,_) -> fea(F,trim(io:get_line(F, ''))).
	
%****************************************************************************************
% Function find_header_ascii(F1), 
%****************************************************************************************
find_header_ascii(F) -> fha(F,"").

fha(_F,"HEADER") -> ok;
fha(_F,eof) -> io:format("Didn't find header section~n",[]),erlang:halt();
fha(F,_) -> fha(F,trim(io:get_line(F, ''))).


%****************************************************************************************
% Function limits_ascii(F)
% $PLIMMAX and $PLIMMIN are for q-cad, set with paper size
%****************************************************************************************
limits_ascii(F) -> limits_ascii(F,{undefined,undefined,undefined,undefined}).

limits_ascii(_F,{X1,Y1,X2,Y2}) when X1 /= undefined andalso X2 /= undefined -> {X1,Y1,X2,Y2};
limits_ascii(F,{X1,Y1,X2,Y2}) ->
	Params = case trim(io:get_line(F, '')) of
		"$EXTMIN" -> trim(io:get_line(F, '')),X = string:to_float(trim(io:get_line(F, ''))),
			trim(io:get_line(F, '')),Y = string:to_float(trim(io:get_line(F, ''))),
			{X,Y,X2,Y2};
		"$PLIMMIN" -> trim(io:get_line(F, '')),X = string:to_float(trim(io:get_line(F, ''))),
			trim(io:get_line(F, '')),Y = string:to_float(trim(io:get_line(F, ''))),
			{X,Y,X2,Y2};
		"$EXTMAX" -> trim(io:get_line(F, '')),X = string:to_float(trim(io:get_line(F, ''))),
			trim(io:get_line(F, '')),Y = string:to_float(trim(io:get_line(F, ''))),
			{X1,Y1,X,Y};
		"$PLIMMAX" -> trim(io:get_line(F, '')),X = string:to_float(trim(io:get_line(F, ''))),
			trim(io:get_line(F, '')),Y = string:to_float(trim(io:get_line(F, ''))),
			{X1,Y1,X,Y};
		eof -> io:format("Didn't find drawing limits~n",[]),erlang:halt();
		_ -> {X1,Y1,X2,Y2}
	end,
	limits_ascii(F,Params).
	
%****************************************************************************************
% Function entities_ascii(Etable,F1)
%****************************************************************************************
entities_ascii(_F,_Etable,_Layer,"ENDSEC") -> ok; 
entities_ascii(_F,_Etable,_Layer,eof) -> ok;
entities_ascii(F,Etable,Layer,E) ->
	entity_ascii(F,Etable,Layer,E),
	entities_ascii(F,Etable,Layer,trim(io:get_line(F, ''))).

%****************************************************************************************
% Function entity_ascii(F,Etable,Entity);
%****************************************************************************************
entity_ascii(F,Etable,Layer,E) ->
	Gtable = ets:new(group,[duplicate_bag,private]),
	reset_all(Gtable),
	e_a(F,Etable,Gtable,Layer,E,trim(io:get_line(F, ''))).

e_a(_F,_Etable,_Gtable,_Layer,_E,eof) -> ok;
e_a(_F,Etable,Gtable,Layer,E,"0") -> % end of this entity
	params(Gtable,Etable,Layer,E,'end',0);
e_a(F,Etable,Gtable,Layer,E,G) ->
	G1 = list_to_integer(G),
	V = format_value(G1,trim(io:get_line(F, ''))), 
	params(Gtable,Etable,Layer,E,V,G1),
	e_a(F,Etable,Gtable,Layer,E,trim(io:get_line(F, ''))).
	
%****************************************************************************************
% Function trim(String) 
%****************************************************************************************
trim(String) ->
    reverse(strip(reverse(strip(String)))).

strip([$   | Cs]) -> strip(Cs);
strip([$\t | Cs]) -> strip(Cs);
strip([$\r | Cs]) -> strip(Cs);
strip([$\n | Cs]) -> strip(Cs);
strip(Cs) -> Cs.

%****************************************************************************
% Check that this is a binary DXF
%****************************************************************************
read_dxf_tag(File) -> 
	case file:open(File, [read,binary,raw]) of
		{ok, S} ->
			{ok, B2} = file:pread(S, 0, 18),
			Result = parse_tag(B2),
			file:close(S);
		 _ -> 	
			Result = error
	end,		 
	Result.

	parse_tag(<<$A,$u,$t,$o,$C,$A,$D,$\s,$B,$i,$n,$a,$r,$y,$\s,$D,$X,$F>>) -> bin; 	
	parse_tag(_) -> ascii. 

%****************************************************************************
% Print all the entities, lowest thickness first
%****************************************************************************
prints_entities(Etable,Ttable) ->
	Elevlist = elevations(Etable),
	prints_entities1(Elevlist,Etable,Ttable).
	
prints_entities1([],_Etable,_Ttable) -> true;
prints_entities1([E|Tail],Etable,Ttable) ->
	Entitytuplelist = ets:lookup(Etable,E),% io:format("Entities ~p~n",[Entitytuplelist]),
	lists:foreach(fun (Entity) -> print_entity(Entity,Ttable) end, Entitytuplelist),
	prints_entities1(Tail,Etable,Ttable).

%****************************************************************************
% Create a sorted list with all unique Thicknesses
%****************************************************************************
elevations(Etable) -> 
	Key = ets:first(Etable),
	Elevlist = elevation1(Etable,[],Key),
	lists:usort(Elevlist).

elevation1(_,Elevlist,'$end_of_table') -> Elevlist;
elevation1(Etable,Elevlist,Key) ->
	Key1 = ets:next(Etable,Key),
	elevation1(Etable,[Key|Elevlist],Key1).

%****************************************************************************
% Math functions
%****************************************************************************
rtod(R) -> R * 180 / pi().

dtor(D) -> D * pi() / 180.

rtodfix(R)-> R1 = R * 180 / pi(),
	 case R1 > 360 of
		true -> R1 - 360;
		_ -> R1
		end.

cotbce(B) -> ((1 / B) - B) / 2.

xcenter(X1,X2,Y1,Y2,Cbce) -> (((Y2 - Y1) * Cbce * -1) + X1 + X2) / 2.

ycenter(X1,X2,Y1,Y2,Cbce) -> (((X2 - X1) * Cbce) + Y1 + Y2) / 2.	

radius(X1,Y1,Xcen,Ycen) -> T = pow((Xcen - X1),2) + pow((Ycen - Y1),2),
	sqrt(T).

ang(X1,Y1,Xcen,Ycen) -> atan2((Y1 - Ycen),(X1 - Xcen)).

fixang(Ang) when Ang < 0.0 -> Ang + (2 * pi());
fixang(Ang) -> Ang.

%****************************************************************************
% Draw an lwpolyline segment
%****************************************************************************
drawSegment([{10,X1}|[{10,X2}|_]],[{20,Y1}|[{20,Y2}|_]],[{42,B1}|_]) when 
		(B1 > 0.000063) or (B1 < -0.000063) -> % Arc ahead
	Cbce = cotbce(B1), 
	Ycen = ycenter(X1,X2,Y1,Y2,Cbce),
	Xcen = xcenter(X1,X2,Y1,Y2,Cbce),
	Rad = radius(X1,Y1,Xcen,Ycen),
	St_ang = fixang(ang(X1,Y1,Xcen,Ycen)), 
	End_ang = fixang(ang(X2,Y2,Xcen,Ycen)),
	io:format("<!-- ctx.arc(~.12f,~.12f,~.12f,~.12f,~.12f,~p); -->~n",[Xcen,Ycen,Rad,St_ang,End_ang,B1<0]); 
drawSegment([{10,X1}|_],[{20,Y1}|_],_) -> 
	io:format("<!-- ctx.lineTo(~.12f,~.12f); -->~n",[X1,Y1]).

%****************************************************************************
% Draw a polyline segment
%****************************************************************************
drawSegment(B2,X1,Y1,X2,Y2) when
			(B2 > 0.000063) or (B2 < -0.000063) -> 
	Cbce = cotbce(B2), 
	Ycen = ycenter(X1,X2,Y1,Y2,Cbce),
	Xcen = xcenter(X1,X2,Y1,Y2,Cbce), 
	Rad = radius(X1,Y1,Xcen,Ycen),
	St_ang = fixang(ang(X1,Y1,Xcen,Ycen)), 
	End_ang = fixang(ang(X2,Y2,Xcen,Ycen)),
	io:format("<!-- ctx.arc(~.12f,~.12f,~.12f,~.12f,~.12f,~p); -->~n",[Xcen,Ycen,Rad,St_ang,End_ang,B2<0]); 
drawSegment(_,_,_,X2,Y2) -> % This is a line segment
	io:format("<!-- ctx.lineTo(~.12f,~.12f);  -->~n",[X2,Y2]).

%****************************************************************************
% Fill or stroke the lwpolyline
%****************************************************************************
doLWPoly(Closed,_FirstVertex,[],[],[]) ->
	case Closed of 
		1 -> io:format("<!-- ctx.fill(); -->~n");
		_ -> io:format("<!-- ctx.stroke(); -->~n")
	end;
doLWPoly(Closed,FirstVertex,G42list,G10list,G20list) ->
	[_|G42tail] = G42list,
	[{10,_X1}|G10tail] = G10list,
	[{20,_Y1}|G20tail] = G20list,
	case FirstVertex of
		1 -> 	io:format("<!-- ctx.beginPath(); -->~n"),
				doLWPoly(Closed,0,G42list,G10list,G20list);
		_ ->  drawSegment(G10list,G20list,G42list),
				doLWPoly(Closed,0,G42tail,G10tail,G20tail)
	end.

%****************************************************************************
% Draw a spline segment
%****************************************************************************
drawSplineSegment([{10,X1}|_],[{20,Y1}|_]) -> 
	io:format("~.12f,~.12f,1 ",[X1,Y1]).

%****************************************************************************
% Fill or stroke the spline
%****************************************************************************
doSpline(Closed,_FirstPoint,[],[]) ->
	case Closed of 
		1 -> io:format("<!-- Spline closed -->~n");
		_ -> io:format(")\"  ~ncolor=\"rgba(0,0,0,1)\"/> ~n")
	end;
doSpline(Closed,FirstPoint,G10list,G20list) ->
	[{10,_X1}|G10tail] = G10list,
	[{20,_Y1}|G20tail] = G20list,
	case FirstPoint of
		1 -> 	I = get(edge),
					io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ",[I]),
					put(edge,I+1),
					doSpline(Closed,0,G10list,G20list);
		_ ->  drawSplineSegment(G10list,G20list),
				doSpline(Closed,0,G10tail,G20tail)
	end.

%****************************************************************************
% Set current drawing color if it is new
%****************************************************************************
setColor(Pen) ->
	case get(color) /= Pen of
   		true -> put(color,Pen),
   			io:format("<!-- Color ~p -->~n",[Pen]);
		_ -> ok
end.
	
%****************************************************************************
% Print an entity
%****************************************************************************
print_entity({_,"TRACE",Entity},_) -> print_entity({0,"SOLID",Entity},0);

print_entity({_,"SOLID",Entity},_) ->
	[{_,X1}|_] = lookup(Entity, 10),[{_,Y1}|_] = lookup(Entity, 20),
	[{_,X2}|_] = lookup(Entity, 12),[{_,Y2}|_] = lookup(Entity, 22),
	[{_,X3}|_] = lookup(Entity, 13),[{_,Y3}|_] = lookup(Entity, 23),
	[{_,X4}|_] = lookup(Entity, 11),[{_,Y4}|_] = lookup(Entity, 21),
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
	setColor(Pen),
	I = get(edge),
	io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ~.12f,~.12f,1 ~.12f,~.12f,1)\" ~ncolor=\"rgba(0,0,0,1)\" />~n",
		[I,X1,Y1,X2,Y2]),
	io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ~.12f,~.12f,1 ~.12f,~.12f,1)\" ~ncolor=\"rgba(0,0,0,1)\" />~n",
		[I+1,X2,Y2,X3,Y3]),
	io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ~.12f,~.12f,1 ~.12f,~.12f,1)\" ~ncolor=\"rgba(0,0,0,1)\" />~n",
		[I+2,X3,Y3,X4,Y4]),
	io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ~.12f,~.12f,1 ~.12f,~.12f,1)\" ~ncolor=\"rgba(0,0,0,1)\" />~n",
		[I+4,X4,Y4,X1,Y1]),
	put(edge,I+4);
	
print_entity({_,"LINE",Entity},_) ->
	[{_,X1}|_] = lookup(Entity, 10),
	[{_,Y1}|_] = lookup(Entity, 20),
	[{_,X2}|_] = lookup(Entity, 11),
	[{_,Y2}|_] = lookup(Entity, 21),
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
	setColor(Pen),
	I = get(edge),
	io:format("<edge ~nid=\"~p\" ~ncurve=\"xywdense(5 ~.12f,~.12f,1 ~.12f,~.12f,1)\" ~ncolor=\"rgba(0,0,0,1)\" />~n",
		[I,X1,Y1,X2,Y2]),
	put(edge,I+1);

print_entity({_,"POINT",Entity},_) ->
	[{_,X1}|_] = lookup(Entity, 10),
	[{_,Y1}|_] = lookup(Entity, 20),
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
	I = get(vertex),
	setColor(Pen),
	io:format("<vertex ~nid=\"~p\" ~nposition=\"~.12f ~.12f\" ~ncolor=\"rgba(0,0,0,1)\" />~n",[I,X1,Y1]),
	put(vertex,I+1);

print_entity({_,"SPLINE",Entity},_) ->
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
 	G10list = lookup(Entity, 10),
 	G20list = lookup(Entity, 20),
	setColor(Pen),
	doSpline(0,1,G10list,G20list);
	
print_entity({_,"ARC",Entity},_) ->
	[{_,_X1}|_] = lookup(Entity, 10),
	[{_,_Y1}|_] = lookup(Entity, 20),
	[{_,_Radius}|_] = lookup(Entity, 40),
	[{_,_Startangle}|_] = lookup(Entity, 50),
	[{_,_Endangle}|_] = lookup(Entity, 51),
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
	setColor(Pen),
	io:format("<!-- ToDo: ARC -->~n",[]);

print_entity({_,"ELLIPSE",Entity},_) -> 
	[{_,_X1}|_] = lookup(Entity, 10),
	[{_,_Y1}|_] = lookup(Entity, 20),
	[{_,RadiusX}|_] = lookup(Entity, 11),
	[{_,RadiusY}|_] = lookup(Entity, 21),
	[{_,_Startangle}|_] = lookup(Entity, 41),
	[{_,_Endangle}|_] = lookup(Entity, 42),
	[{_,_Ratio}|_] = lookup(Entity, 40),
	[{_,Pen}|_] = reverse(lookup(Entity, 62)),
	_Rot = ang(RadiusX,RadiusY,0,0),
	_R = radius(RadiusX,RadiusY,0,0),
	setColor(Pen),
	io:format("<!-- ToDo: ELLIPSE -->~n",[]);

print_entity({_,"CIRCLE",Entity},_) ->
	[{_,_X1}|_] = lookup(Entity, 10),
  [{_,_Y1}|_] = lookup(Entity, 20),
	[{_,_Radius}|_] = lookup(Entity, 40),
  [{_,Pen}|_] = reverse(lookup(Entity, 62)),
	setColor(Pen),
	io:format("<!-- ToDo: CIRCLE -->~n",[]);

print_entity({_,"POLYLINE",Entity},Ttable) -> 
	[{_,Pen}|_] = reverse(lookup(Entity, 62)), 
	setColor(Pen),
	Closed = lookup_safe(Entity, 70), 
	insert(Ttable,{firstvertex,1}),
	insert(Ttable,{flags,Closed});
	
print_entity({_,"VERTEX",Entity},Ttable) -> 
	[{_,X1}|_] = lookup(Entity, 10),
	[{_,Y1}|_] = lookup(Entity, 20),
	[{_,FV}|_] = lookup(Ttable, firstvertex),
	Bulge = lookup_safe(Entity, 42),
	case FV of	
		1 ->  	
			io:format("<!-- ToDo: First VERTEX -->~n",[]),
			insert(Ttable,{firstvertex,0}),
			insert(Ttable,{startx,X1}),
			insert(Ttable,{starty,Y1});
		_ -> 	
			[{_,X2}|_] = lookup(Ttable, 10), 
			[{_,Y2}|_] = lookup(Ttable, 20),
			Bulge1 = lookup_safe(Ttable,42), % Bulge group on previous vertex
			drawSegment(Bulge1,X2,Y2,X1,Y1)
	end,
	insert(Ttable,{10,X1}),    % Save point
	insert(Ttable,{20,Y1}),
	insert(Ttable,{42,Bulge}); % Save bulge
	

print_entity({_,"SEQEND",_Entity},Ttable) ->
	[{_,Closed}|_] = lookup(Ttable, flags),
	case Closed of
		0 -> 	io:format("<!-- ToDo: SEQEND -->~n",[]);
		1 -> 	[{_,X1}|_] = lookup(Ttable, startx), % Draw the closing segment
		 		[{_,Y1}|_] = lookup(Ttable, starty),
				[{_,X2}|_] = lookup(Ttable, 10), 
				[{_,Y2}|_] = lookup(Ttable, 20),
				Bulge1 = lookup_safe(Ttable,42),
				drawSegment(Bulge1,X2,Y2,X1,Y1),
				io:format("<!-- ToDo: SEQEND -->~n",[])
	end,
	ets:delete_all_objects(Ttable);

print_entity({_,"LWPOLYLINE",Entity},_) ->
	[{_,Pen}|_] = reverse(lookup(Entity, 62)), 
	setColor(Pen),
	G10list = lookup(Entity, 10),
	G20list = lookup(Entity, 20),
	fixBulgelist(Entity,length(lookup(Entity, 10)),length(lookup(Entity, 42))),
	Bulgelist = lookup(Entity, 42),
	[{70,Closed}] = lookup(Entity, 70),
	case Closed of
		1 -> 	G10list1 = append_startpoint(G10list),
		  		G20list1 = append_startpoint(G20list),
		  		Bulgelist1 = append_startpoint(Bulgelist);
		_ -> 	G10list1 = G10list,
	   		G20list1 = G20list,
	   		Bulgelist1 = Bulgelist
	end,
   doLWPoly(Closed,1,Bulgelist1,G10list1,G20list1);

print_entity({_,_Name,_Entity},_) -> ok.
%		List = ets:tab2list(_Entity),
%		io:format("~p ~p~n", [_Name,List]).

% Fill up with G42's until list length is same as G10's
fixBulgelist(_Entity,X,X) -> ok;
fixBulgelist(Entity,X,Y) -> insert(Entity, {42,0}), fixBulgelist(Entity,X,Y+1).

% If an Lwpline is closed we need to append the last path segment
append_startpoint(List) ->
	L = hd(List),
	L1 = reverse(List),
	L2 = [L|L1],
	reverse(L2).

% Some group codes are optional. Return a zero if not found.
lookup_safe(Entity,G) -> 
	case  lookup(Entity, G) of
		[{_,Bulge}|_] -> Bulge;
		_ -> 0
	end.

%****************************************************************************
% Find the header section in the dxf file
%****************************************************************************
find_header(B) -> find_header1({B,"",0}).

find_header1({B,"HEADER",2}) -> B;
find_header1({B,_,_}) -> find_header1(parse_dxf(B)).

%****************************************************************************
% Print the header section in the vec file
%****************************************************************************
print_header() ->
	io:format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>~n",[]),
	io:format("<!-- Created with dxf2vec -->~n~n",[]),
	io:format("<vec ~nversion=\"1.7\">~n",[]),
	io:format("<playback ~nframerange=\"0 47\" ~nfps=\"24\" ~nsubframeinbetweening=\"off\" ~nplaymode=\"normal\"/>~n",[]),
	io:format("<canvas ~nposition=\"0 0\" ~nsize=\"400 200\"/>~n",[]),
	io:format("<layer ~nname=\"Layer 1\" ~nvisible=\"true\">~n",[]),
	io:format("<background ~ncolor=\"rgba(255,255,255,1)\" ~nimage=\"\" ~nposition=\"0 0\"~n",[]), 
	io:format("size=\"cover\" ~nrepeat=\"norepeat\" ~nopacity=\"1\" ~nhold=\"yes\"/>~n",[]),
	io:format("<objects>~n",[]).
	
%****************************************************************************
% Write the size of the drawing
%****************************************************************************
limits(B) -> limits1({B,"",0}).

limits1({B,"$EXTMIN",9}) -> 
	{B1,X1,_G1} = parse_dxf(B), 
	{B2,Y1,_G2} = parse_dxf(B1),
    io:format("<!-- bbox = [~B,~B,",[round(X1),round(Y1)]),
	limits1({B2,"",0});
limits1({B,"$EXTMAX",9}) -> 
	{B1,X2,_G1} = parse_dxf(B), 
	{_B2,Y2,_G2} = parse_dxf(B1),
	io:format("~B,~B] -->~n",[round(X2),round(Y2)]);
limits1({B,_,_}) -> limits1(parse_dxf(B)).

%****************************************************************************
% Insert a new entity table Gtable in the Etable. Thickness is the key, Etype is the entity type
%****************************************************************************
doInsert(Gtable,Etable,Etype,Layer)  -> 
	[{8,Layer1}|_] = reverse(lookup(Gtable, 8)), % Check it's on the right layer
	case Layer == Layer1 of
		true ->
				[{39,Elev}|_] = reverse(lookup(Gtable, 39)),
				case Etype == "POLYLINE" of  % A hack for polylines when vertices and
					true -> 	put(elevation,Elev); % seqend don't have a thickness
					_ -> 		ok
				end,
				case ((Etype == "VERTEX") or (Etype == "SEQEND")) of
					true -> 	insert(Etable,{get(elevation),Etype,Gtable});
					_ -> 		insert(Etable,{Elev,Etype,Gtable})
				end;
		_ -> ignore
	end.

%****************************************************************************
% Erase the previous entity from the Gtable and set some default values
%****************************************************************************
reset_all(Gtable) ->
	ets:delete_all_objects(Gtable),
	insert(Gtable, {6,"CONTINUOUS"}),
	insert(Gtable, {7,"STANDARD"}),
	insert(Gtable, {8,"0"}),
	insert(Gtable, {38,0}),
	insert(Gtable, {39,0}),
	insert(Gtable, {44,0}),
	insert(Gtable, {45,0}),
	insert(Gtable, {62,0}),
	insert(Gtable, {71,0}),
	insert(Gtable, {72,0}).
	
%****************************************************************************
% Find the entities section in the dxf file
%****************************************************************************
find_entities(B) -> find_entities1({B,"",0}).
find_entities1({B,"ENTITIES",2}) -> B;
find_entities1({B,_,_}) -> find_entities1(parse_dxf(B)).

%****************************************************************************
% Step thru each entity in the entities section
%****************************************************************************
entities(_,<<>>,_Layer) -> true;
entities(Etable,<<0:?WORD,Rest/binary>>,Layer) -> 
	Gtable = ets:new(group,[duplicate_bag,private]),
	reset_all(Gtable), 
	{B,T} = ac_text(Rest), %io:format("In entities: T= ~p~n",[T]),
	B1 = entity(Gtable,Etable,Layer,T,B), 
	entities(Etable,B1,Layer);
entities(Etable,B,Layer) -> 
	{B1,_T1,_G1} = parse_dxf(B), 
	entities(Etable,B1,Layer).

%****************************************************************************
% Step thru each Group in an entity
%****************************************************************************
entity(_,_,_,_,<<>>) -> <<>>;
entity(Gtable,Etable,Layer,E,<<0:?WORD,Rest/binary>>) -> 
	params(Gtable,Etable,Layer,E,'end',0), 
	<<0:?WORD,Rest/binary>>; %% Reached the end of current entity
entity(Gtable,Etable,Layer,E,B) ->
	{B1,V1,G1} = parse_dxf(B), %io:format("In entity: ~p ~p~n",[V1,G1]),
	params(Gtable,Etable,Layer,E,V1,G1), 
	entity(Gtable,Etable,Layer,E,B1).

%****************************************************************************
% Insert each entity into an ets table Gtable
%****************************************************************************
params(Gtable,Etable,Layer,Etype, 'end', 0) -> doInsert(Gtable,Etable,Etype,Layer);
%params(Gtable,_,_,_, V, 42) -> 
%	insertBulge(Gtable, length(lookup(Gtable,10)),length(lookup(Gtable,42)), {42,V});
params(Gtable,_,_,_, V, G) -> insert(Gtable, {G,V}).

% When inserting a G42, first make sure the G42 list is equally long (-1) as the G10 list 
insertBulge(Gtable,G10,G42,G) when G10 == G42+1 -> insert(Gtable, G);
insertBulge(Gtable,G10,G42,G) -> insert(Gtable, {42,0}),insertBulge(Gtable,G10,G42+1,G).


%****************************************************************************
% Parse the binary dxf file and create erlang data types
%****************************************************************************
ac_text(Thefile) -> T = parse_text(Thefile, []), {_,B1} = split_binary(Thefile, length(T)+1), {B1,T}.

parse_text(<<0:?BYTE,_R/binary>>,Text) -> reverse(Text) ; %% Terminating zero string composer	
parse_text(<<C:?BYTE,R/binary>>,R1) -> parse_text(R, [C|R1]).

ac_double(Thefile) -> <<Y:?DOUBLE,B/binary>> = Thefile, {B,Y}.

% ac_byte(Thefile) -> <<F:?BYTE,B/binary>> = Thefile, {B,F}.

ac_word(Thefile) -> <<F:?WORD,B/binary>> = Thefile, {B,F}.

ac_dword(Thefile) -> <<F:?DWORD,B/binary>> = Thefile, {B,F}.

parse_dxf(<<G0:?WORD,Rest/binary>>) when G0 >= 0, G0 =< 9 -> {B,T} = ac_text(Rest), {B,T,G0};
parse_dxf(<<G10:?WORD,Rest/binary>>) when G10 >= 10, G10 =< 59 -> {B,F} = ac_double(Rest),{B,F,G10};
parse_dxf(<<G60:?WORD,Rest/binary>>) when G60 >= 60, G60 =< 79 -> {B,W} = ac_word(Rest),{B,W,G60};
parse_dxf(<<G90:?WORD,Rest/binary>>) when G90 >= 90, G90 =< 99 -> {B,W} = ac_dword(Rest),{B,W,G90};
parse_dxf(<<G100:?WORD,Rest/binary>>) when G100 >= 100, G100 =< 105 -> {B,T} = ac_text(Rest),{B,T,G100};
parse_dxf(<<G140:?WORD,Rest/binary>>) when G140 >= 140, G140 =< 147 -> {B,F} = ac_double(Rest),{B,F,G140};
parse_dxf(<<G170:?WORD,Rest/binary>>) when G170 >= 170, G170 =< 178 -> {B,W} = ac_word(Rest),{B,W,G170};
parse_dxf(<<G210:?WORD,Rest/binary>>) when G210 >= 210, G210 =< 239 -> {B,F} = ac_double(Rest),{B,F,G210};
parse_dxf(<<255:?WORD,Rest/binary>>) -> {B,W} = ac_word(Rest),{B,W,255};
parse_dxf(<<G270:?WORD,Rest/binary>>) when G270 >= 270, G270 =< 275 -> {B,W} = ac_word(Rest),{B,W,G270};
parse_dxf(<<G280:?WORD,Rest/binary>>) when G280 >= 280, G280 =< 289 -> {B,W} = ac_word(Rest),{B,W,G280}; 
parse_dxf(<<G300:?WORD,Rest/binary>>) when G300 >= 300, G300 =< 369 -> {B,T} = ac_text(Rest),{B,T,G300};
parse_dxf(<<G1000:?WORD,Rest/binary>>) when G1000 >= 1000, G1000 =< 1009 -> {B,T} = ac_text(Rest),{B,T,G1000};
parse_dxf(<<G1010:?WORD,Rest/binary>>) when G1010 >= 1010, G1010 =< 1059 -> {B,F} = ac_double(Rest),{B,F,G1010};
parse_dxf(<<G1060:?WORD,Rest/binary>>) when G1060 >= 1060, G1060 =< 1069 -> {B,F} = ac_word(Rest),{B,F,G1060};
parse_dxf(<<G1071:?WORD,Rest/binary>>) when G1071 >= 1071, G1071 =< 1079 -> {B,F} = ac_dword(Rest),{B,F,G1071}.
%parse_dxf(<<G:?WORD,_Rest/binary>>) -> io:format("Missing group: ~w~n",[G]), erlang:halt().

%****************************************************************************************
% Parse the ASCII dxf file and create erlang data types
%****************************************************************************************
format_value(G,S) when G >= 0, G =< 9 -> S; %String
format_value(G,S) when G >= 10, G =< 59 -> {V,_} = string:to_float(S),V;
format_value(G,S) when G >= 60, G =< 79 -> list_to_integer(S);
format_value(G,S) when G >= 90, G =< 99 -> list_to_integer(S);
format_value(100,S) -> S; %String
format_value(102,S) -> S; %String
format_value(105,S) -> S; %String
format_value(G,S) when G >= 140, G =< 147 -> {V,_} = string:to_float(S),V;
format_value(G,S) when G >= 170, G =< 175 -> list_to_integer(S);
format_value(G,S) when G >= 210, G =< 230 -> {V,_} = string:to_float(S),V; % ellipse, ej i standard
format_value(G,S) when G >= 280, G =< 289 -> list_to_integer(S);
format_value(G,S) when G >= 300, G =< 369 -> S; %String
format_value(G,S) when G >= 370, G =< 389 -> list_to_integer(S);
format_value(G,S) when G >= 390, G =< 399 -> S; %String
format_value(G,S) when G >= 400, G =< 409 -> list_to_integer(S);
format_value(G,S) when G >= 410, G =< 419 -> S; %String
format_value(420,S) -> list_to_integer(S); % QCAD: some value if entity color is other than default
format_value(999,S) -> S; %String
format_value(G,S) when G >= 1000, G =< 1009 -> S; %String
format_value(G,S) when G >= 1010, G =< 1059 -> {V,_} = string:to_float(S),V;
format_value(G,S) when G >= 1060, G =< 1071 -> list_to_integer(S).
%format_value(G,S) -> io:format("Group: ~p, Value:  ~p~n",[G,S]).

