// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef FRAMERANGE_H
#define FRAMERANGE_H

#include "Frame.h"

/// \class FrameRange
/// \brief A class template to represent a frame range.
///
/// The template parameter Frame must support:
///   - comparison operators <, >, and ==
///   - integer assignment  (frame = 0 and frame = 1 must be allowed)
///
template <class Frame>
class TFrameRange
{
public:
    /// Creates an empty FrameRange.
    ///
    TFrameRange()
    {
        setEmpty();
    }

    /// Creates a FrameRange made of a single frame.
    ///
    TFrameRange(Frame frame)
    {
        setFrame(frame);
    }

    /// Creates a FrameRange given by firstFrame and lastFrame, using the
    /// same semantics of setRange().
    ///
    TFrameRange(Frame firstFrame, Frame lastFrame, bool allowEmpty = false)
    {
        setRange(firstFrame, lastFrame, allowEmpty);
    }

    /// Sets the FrameRange to be empty.
    ///
    void setEmpty()
    {
        firstFrame_ = 1;
        lastFrame_ = 0;
    }

    /// Sets the FrameRange to be a single frame.
    ///
    void setFrame(Frame frame)
    {
        firstFrame_ = frame;
        lastFrame_ = frame;
    }

    /// Sets the FrameRange to be the range given by firstFrame and lastFrame.
    /// If firstFrame < lastFrame, then the FrameRange will be multiple frames.
    /// If firstFrame == lastFrame, then the FrameRange will be made of a single frame.
    /// If firstFrame > lastFrame, then if allowEmpty == false (the default value), firstFrame and lastFrame
    /// are swapped, otherwise the FrameRange will be empty.
    ///
    void setRange(Frame firstFrame, Frame lastFrame, bool allowEmpty = false)
    {
        if (firstFrame > lastFrame)
        {
            if (allowEmpty)
            {
                setEmpty();
            }
            else
            {
                firstFrame_ = lastFrame;
                lastFrame_ = firstFrame;
            }
        }
        else
        {
            firstFrame_ = firstFrame;
            lastFrame_ = lastFrame;
        }
    }

    /// Sets the first frame of the FrameRange.
    /// If the given \p firstFrame is > lastFrame(), then if allowEmpty == false (the default value), firstFrame() becomes
    /// lastFrame(), otherwise the frameRange becomes empty.
    ///
    void setFirstFrame(Frame firstFrame, bool allowEmpty = false)
    {
        if (firstFrame > lastFrame())
        {
            if (allowEmpty)
            {
                setEmpty();
            }
            else
            {
                firstFrame_ = lastFrame();
            }
        }
        else
        {
            firstFrame_ = firstFrame;
        }
    }

    /// Sets the last frame of the FrameRange.
    /// If the given \p lastFrame is < firstFrame(), then if allowEmpty == false (the default value), lastFrame() becomes
    /// firstFrame(), otherwise the frameRange becomes empty.
    ///
    void setLastFrame(Frame lastFrame, bool allowEmpty = false)
    {
        if (firstFrame() > lastFrame)
        {
            if (allowEmpty)
            {
                setEmpty();
            }
            else
            {
                lastFrame_ = firstFrame();
            }
        }
        else
        {
            lastFrame_ = lastFrame;
        }
    }

    /// Returns true if the two FrameRanges are equal. Returns false otherwise.
    ///
    bool operator==(const TFrameRange & other) const
    {
        return firstFrame_ == other.firstFrame_ &&
               lastFrame_  == other.lastFrame_;
    }

    /// Returns true if the two FrameRanges are different. Returns false
    /// otherwise.
    ///
    bool operator!=(const TFrameRange & other) const
    {
        return !(*this == other);
    }

    /// Returns whether the FrameRange is empty.
    ///
    bool isEmpty() const
    {
        return firstFrame_ > lastFrame_;
    }

    /// Returns whether the FrameRange is is made of a single frame.
    ///
    bool isSingleFrame() const
    {
        return firstFrame_ == lastFrame_;
    }

    /// Returns the frame if isSingleFrame() == true, returns an undefined Frame
    /// otherwise.
    ///
    Frame frame() const
    {
        return firstFrame_;
    }

    /// Returns whether the FrameRange is non-empty and not made of a single
    /// frame.
    ///
    bool isMultipleFrame() const
    {
        return !isEmpty() && !isSingleFrame();
    }

    /// Returns the first Frame of the range. Undefined if the range is empty.
    /// Note that firstFrame() == lastFrame() if isSingleFrame() is true.
    /// Same as frame() if isSingleFrame() == true.
    ///
    Frame firstFrame() const
    {
        return firstFrame_;
    }

    /// Returns the last Frame of the range. Undefined if the range is empty.
    /// Note that firstFrame() == lastFrame() if isSingleFrame() is true.
    /// Same as frame() if isSingleFrame() == true.
    ///
    Frame lastFrame() const
    {
        return lastFrame_;
    }

private:
    Frame firstFrame_;
    Frame lastFrame_;
};

using IntFrameRange = TFrameRange<IntFrame>;
using DoubleFrameRange = TFrameRange<DoubleFrame>;
using FrameRange = TFrameRange<Frame>;

#endif // FRAMERANGE_H
