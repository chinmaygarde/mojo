/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SKY_ENGINE_CORE_ANIMATION_KEYFRAMEEFFECTMODEL_H_
#define SKY_ENGINE_CORE_ANIMATION_KEYFRAMEEFFECTMODEL_H_

#include "sky/engine/core/animation/AnimationEffect.h"
#include "sky/engine/core/animation/AnimationNode.h"
#include "sky/engine/core/animation/InterpolationEffect.h"
#include "sky/engine/core/animation/StringKeyframe.h"
#include "sky/engine/core/animation/animatable/AnimatableValueKeyframe.h"
#include "sky/engine/platform/animation/TimingFunction.h"
#include "sky/engine/wtf/HashMap.h"
#include "sky/engine/wtf/PassOwnPtr.h"
#include "sky/engine/wtf/PassRefPtr.h"
#include "sky/engine/wtf/Vector.h"

namespace blink {

class Element;
class KeyframeEffectModelTest;

class KeyframeEffectModelBase : public AnimationEffect {
public:
    // FIXME: Implement accumulation.

    typedef Vector<OwnPtr<Keyframe::PropertySpecificKeyframe> > PropertySpecificKeyframeVector;
    class PropertySpecificKeyframeGroup {
    public:
        void appendKeyframe(PassOwnPtr<Keyframe::PropertySpecificKeyframe>);
        const PropertySpecificKeyframeVector& keyframes() const { return m_keyframes; }

    private:
        void removeRedundantKeyframes();
        void addSyntheticKeyframeIfRequired(const KeyframeEffectModelBase* context);

        PropertySpecificKeyframeVector m_keyframes;

        friend class KeyframeEffectModelBase;
    };

    bool isReplaceOnly();

    PropertySet properties() const;

    typedef Vector<RefPtr<Keyframe> > KeyframeVector;
    const KeyframeVector& getFrames() const { return m_keyframes; }
    // FIXME: Implement setFrames()

    const PropertySpecificKeyframeVector& getPropertySpecificKeyframes(CSSPropertyID id) const
    {
        ensureKeyframeGroups();
        return m_keyframeGroups->get(id)->keyframes();
    }

    // AnimationEffect implementation.
    virtual PassOwnPtr<Vector<RefPtr<Interpolation> > > sample(int iteration, double fraction, double iterationDuration) const override;

    virtual bool isKeyframeEffectModel() const override { return true; }

    virtual bool isAnimatableValueKeyframeEffectModel() const { return false; }
    virtual bool isStringKeyframeEffectModel() const { return false; }

    // FIXME: This is a hack used to resolve CSSValues to AnimatableValues while we have a valid handle on an element.
    // This should be removed once StringKeyframes no longer uses InterpolableAnimatableValues.
    void forceConversionsToAnimatableValues(Element* element)
    {
        ensureKeyframeGroups();
        ensureInterpolationEffect(element);
    }

protected:
    static KeyframeVector normalizedKeyframes(const KeyframeVector& keyframes);

    // Lazily computes the groups of property-specific keyframes.
    void ensureKeyframeGroups() const;
    void ensureInterpolationEffect(Element* = 0) const;

    KeyframeVector m_keyframes;
    // The spec describes filtering the normalized keyframes at sampling time
    // to get the 'property-specific keyframes'. For efficiency, we cache the
    // property-specific lists.
    typedef HashMap<CSSPropertyID, OwnPtr<PropertySpecificKeyframeGroup> > KeyframeGroupMap;
    mutable OwnPtr<KeyframeGroupMap> m_keyframeGroups;
    mutable RefPtr<InterpolationEffect> m_interpolationEffect;

    friend class KeyframeEffectModelTest;

    bool affects(CSSPropertyID property)
    {
        ensureKeyframeGroups();
        return m_keyframeGroups->contains(property);
    }
};

template <class Keyframe>
class KeyframeEffectModel final : public KeyframeEffectModelBase {
public:
    typedef Vector<RefPtr<Keyframe> > KeyframeVector;
    static PassRefPtr<KeyframeEffectModel<Keyframe> > create(const KeyframeVector& keyframes) { return adoptRef(new KeyframeEffectModel(keyframes)); }

private:
    KeyframeEffectModel(const KeyframeVector& keyframes)
    {
        m_keyframes.appendVector(keyframes);
    }

    virtual bool isAnimatableValueKeyframeEffectModel() const { return false; }
    virtual bool isStringKeyframeEffectModel() const { return false; }

};

typedef KeyframeEffectModelBase::KeyframeVector KeyframeVector;
typedef KeyframeEffectModelBase::PropertySpecificKeyframeVector PropertySpecificKeyframeVector;

typedef KeyframeEffectModel<AnimatableValueKeyframe> AnimatableValueKeyframeEffectModel;
typedef AnimatableValueKeyframeEffectModel::KeyframeVector AnimatableValueKeyframeVector;
typedef AnimatableValueKeyframeEffectModel::PropertySpecificKeyframeVector AnimatableValuePropertySpecificKeyframeVector;

typedef KeyframeEffectModel<StringKeyframe> StringKeyframeEffectModel;
typedef StringKeyframeEffectModel::KeyframeVector StringKeyframeVector;
typedef StringKeyframeEffectModel::PropertySpecificKeyframeVector StringPropertySpecificKeyframeVector;

DEFINE_TYPE_CASTS(KeyframeEffectModelBase, AnimationEffect, value, value->isKeyframeEffectModel(), value.isKeyframeEffectModel());
DEFINE_TYPE_CASTS(AnimatableValueKeyframeEffectModel, KeyframeEffectModelBase, value, value->isAnimatableValueKeyframeEffectModel(), value.isAnimatableValueKeyframeEffectModel());

inline const AnimatableValueKeyframeEffectModel* toAnimatableValueKeyframeEffectModel(const AnimationEffect* base)
{
    return toAnimatableValueKeyframeEffectModel(toKeyframeEffectModelBase(base));
}

inline AnimatableValueKeyframeEffectModel* toAnimatableValueKeyframeEffectModel(AnimationEffect* base)
{
    return toAnimatableValueKeyframeEffectModel(toKeyframeEffectModelBase(base));
}

template <>
inline bool KeyframeEffectModel<AnimatableValueKeyframe>::isAnimatableValueKeyframeEffectModel() const { return true; }

template <>
inline bool KeyframeEffectModel<StringKeyframe>::isStringKeyframeEffectModel() const { return true; }

} // namespace blink

#endif  // SKY_ENGINE_CORE_ANIMATION_KEYFRAMEEFFECTMODEL_H_
