/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2009 - 2010  Torch Mobile (Beijing) Co. Ltd. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SKY_ENGINE_CORE_CSS_PARSER_CSSPROPERTYPARSER_H_
#define SKY_ENGINE_CORE_CSS_PARSER_CSSPROPERTYPARSER_H_

// FIXME: Way too many.
#include "gen/sky/core/CSSPropertyNames.h"
#include "gen/sky/core/CSSValueKeywords.h"
#include "sky/engine/core/css/CSSCalculationValue.h"
#include "sky/engine/core/css/CSSFilterValue.h"
#include "sky/engine/core/css/CSSGradientValue.h"
#include "sky/engine/core/css/CSSProperty.h"
#include "sky/engine/core/css/CSSPropertySourceData.h"
#include "sky/engine/core/css/CSSSelector.h"
#include "sky/engine/core/css/parser/CSSParserMode.h"
#include "sky/engine/core/css/parser/CSSParserValues.h"
#include "sky/engine/platform/graphics/Color.h"
#include "sky/engine/wtf/OwnPtr.h"
#include "sky/engine/wtf/Vector.h"

namespace blink {

class CSSBorderImageSliceValue;
class CSSPrimitiveValue;
class CSSValue;
class CSSValueList;
class CSSBasicShape;
class CSSBasicShapeInset;
class ImmutableStylePropertySet;
class StylePropertyShorthand;

// Inputs: PropertyID, CSSParserValueList.
// Outputs: Vector of CSSProperties

class CSSPropertyParser {
    STACK_ALLOCATED();
public:
    static bool parseValue(CSSPropertyID,
        CSSParserValueList*, const CSSParserContext&, bool inViewport,
        Vector<CSSProperty, 256>&, CSSRuleSourceData::Type);

    // FIXME: Should this be on a separate ColorParser object?
    template<typename StringType>
    static bool fastParseColor(RGBA32&, const StringType&, bool strict);

    static bool isSystemColor(int id);

private:
    CSSPropertyParser(CSSParserValueList*, const CSSParserContext&, bool inViewport,
        Vector<CSSProperty, 256>&, CSSRuleSourceData::Type);

    bool parseValue(CSSPropertyID);

    bool inShorthand() const { return m_inParseShorthand; }
    bool inQuirksMode() const { return isQuirksModeBehavior(m_context.mode()); }

    bool inViewport() const { return m_inViewport; }
    bool parseViewportProperty(CSSPropertyID propId);
    bool parseViewportShorthand(CSSPropertyID propId, CSSPropertyID first, CSSPropertyID second);

    KURL completeURL(const String& url) const;

    void addProperty(CSSPropertyID, PassRefPtr<CSSValue>, bool implicit = false);
    void rollbackLastProperties(int num);
    void addExpandedPropertyForValue(CSSPropertyID propId, PassRefPtr<CSSValue>);

    PassRefPtr<CSSPrimitiveValue> parseValidPrimitive(CSSValueID ident, CSSParserValue*);

    bool parseShorthand(CSSPropertyID, const StylePropertyShorthand&);
    bool parse4Values(CSSPropertyID, const CSSPropertyID* properties);

    PassRefPtr<CSSValue> parseAttr(CSSParserValueList* args);

    PassRefPtr<CSSValue> parseBackgroundColor();

    bool parseFillImage(CSSParserValueList*, RefPtr<CSSValue>&);

    enum FillPositionFlag { InvalidFillPosition = 0, AmbiguousFillPosition = 1, XFillPosition = 2, YFillPosition = 4 };
    enum FillPositionParsingMode { ResolveValuesAsPercent = 0, ResolveValuesAsKeyword = 1 };
    PassRefPtr<CSSPrimitiveValue> parseFillPositionComponent(CSSParserValueList*, unsigned& cumulativeFlags, FillPositionFlag& individualFlag, FillPositionParsingMode = ResolveValuesAsPercent);
    PassRefPtr<CSSValue> parseFillPositionX(CSSParserValueList*);
    PassRefPtr<CSSValue> parseFillPositionY(CSSParserValueList*);
    void parse2ValuesFillPosition(CSSParserValueList*, RefPtr<CSSValue>&, RefPtr<CSSValue>&);
    bool isPotentialPositionValue(CSSParserValue*);
    void parseFillPosition(CSSParserValueList*, RefPtr<CSSValue>&, RefPtr<CSSValue>&);
    void parse3ValuesFillPosition(CSSParserValueList*, RefPtr<CSSValue>&, RefPtr<CSSValue>&, PassRefPtr<CSSPrimitiveValue>, PassRefPtr<CSSPrimitiveValue>);
    void parse4ValuesFillPosition(CSSParserValueList*, RefPtr<CSSValue>&, RefPtr<CSSValue>&, PassRefPtr<CSSPrimitiveValue>, PassRefPtr<CSSPrimitiveValue>);

    void parseFillRepeat(RefPtr<CSSValue>&, RefPtr<CSSValue>&);
    PassRefPtr<CSSValue> parseFillSize(CSSPropertyID, bool &allowComma);

    bool parseFillProperty(CSSPropertyID propId, CSSPropertyID& propId1, CSSPropertyID& propId2, RefPtr<CSSValue>&, RefPtr<CSSValue>&);
    bool parseFillShorthand(CSSPropertyID, const CSSPropertyID* properties, int numProperties);

    void addFillValue(RefPtr<CSSValue>& lval, PassRefPtr<CSSValue> rval);

    PassRefPtr<CSSValue> parseAnimationDelay();
    PassRefPtr<CSSValue> parseAnimationDirection();
    PassRefPtr<CSSValue> parseAnimationDuration();
    PassRefPtr<CSSValue> parseAnimationFillMode();
    PassRefPtr<CSSValue> parseAnimationIterationCount();
    PassRefPtr<CSSValue> parseAnimationName();
    PassRefPtr<CSSValue> parseAnimationPlayState();
    PassRefPtr<CSSValue> parseAnimationProperty();
    PassRefPtr<CSSValue> parseAnimationTimingFunction();

    bool parseWebkitTransformOriginShorthand();
    bool parseCubicBezierTimingFunctionValue(CSSParserValueList*& args, double& result);
    PassRefPtr<CSSValue> parseAnimationProperty(CSSPropertyID);
    PassRefPtr<CSSValueList> parseAnimationPropertyList(CSSPropertyID);
    bool parseTransitionShorthand(CSSPropertyID);
    bool parseAnimationShorthand(CSSPropertyID);

    bool parseClipShape(CSSPropertyID);

    bool parseItemPositionOverflowPosition(CSSPropertyID);

    PassRefPtr<CSSValue> parseShapeProperty(CSSPropertyID propId);
    PassRefPtr<CSSValue> parseBasicShapeAndOrBox();
    PassRefPtr<CSSPrimitiveValue> parseBasicShape();
    PassRefPtr<CSSPrimitiveValue> parseShapeRadius(CSSParserValue*);

    PassRefPtr<CSSBasicShape> parseBasicShapeCircle(CSSParserValueList* args);
    PassRefPtr<CSSBasicShape> parseBasicShapeEllipse(CSSParserValueList* args);
    PassRefPtr<CSSBasicShape> parseBasicShapePolygon(CSSParserValueList* args);
    PassRefPtr<CSSBasicShape> parseBasicShapeInset(CSSParserValueList* args);

    bool parseFont();
    PassRefPtr<CSSValueList> parseFontFamily();

    bool parseColorParameters(CSSParserValue*, int* colorValues, bool parseAlpha);
    bool parseHSLParameters(CSSParserValue*, double* colorValues, bool parseAlpha);
    PassRefPtr<CSSPrimitiveValue> parseColor(CSSParserValue* = 0, bool acceptQuirkyColors = false);
    bool parseColorFromValue(CSSParserValue*, RGBA32&, bool acceptQuirkyColors = false);

    bool parseLineHeight();
    bool parseFontSize();
    bool parseFontVariant();
    bool parseFontWeight();
    PassRefPtr<CSSValueList> parseFontFaceUnicodeRange();

    // CSS3 Parsing Routines (for properties specific to CSS3)
    PassRefPtr<CSSValueList> parseShadow(CSSParserValueList*, CSSPropertyID);
    bool parseBorderImageShorthand(CSSPropertyID);
    PassRefPtr<CSSValue> parseBorderImage(CSSPropertyID);
    bool parseBorderImageRepeat(RefPtr<CSSValue>&);
    bool parseBorderImageSlice(CSSPropertyID, RefPtr<CSSBorderImageSliceValue>&);
    bool parseBorderImageWidth(RefPtr<CSSPrimitiveValue>&);
    bool parseBorderImageOutset(RefPtr<CSSPrimitiveValue>&);
    bool parseBorderRadius(CSSPropertyID);

    PassRefPtr<CSSValue> parseAspectRatio();

    bool parseFlex(CSSParserValueList* args);

    PassRefPtr<CSSValue> parseObjectPosition();

    bool parseLinearGradient(CSSParserValueList*, RefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseRadialGradient(CSSParserValueList*, RefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseGradientColorStops(CSSParserValueList*, CSSGradientValue*, bool expectComma);

    PassRefPtr<CSSValue> parseWillChange();

    PassRefPtr<CSSValueList> parseFilter();
    PassRefPtr<CSSFilterValue> parseBuiltinFilterArguments(CSSParserValueList*, CSSFilterValue::FilterOperationType);

    PassRefPtr<CSSValueList> parseTransformOrigin();
    PassRefPtr<CSSValueList> parseTransform(CSSPropertyID);
    PassRefPtr<CSSValue> parseTransformValue(CSSPropertyID, CSSParserValue*);

    bool parseTextEmphasisStyle();

    PassRefPtr<CSSValue> parseTouchAction();

    void addTextDecorationProperty(CSSPropertyID, PassRefPtr<CSSValue>);
    bool parseTextDecoration(CSSPropertyID propId);
    bool parseTextUnderlinePosition();

    PassRefPtr<CSSValue> parseTextIndent();

    bool parseLineBoxContain();
    bool parseCalculation(CSSParserValue*, ValueRange);

    bool parseFontFeatureTag(CSSValueList*);
    bool parseFontFeatureSettings();

    bool parseFontVariantLigatures();

    bool parseGeneratedImage(CSSParserValueList*, RefPtr<CSSValue>&);

    PassRefPtr<CSSPrimitiveValue> createPrimitiveNumericValue(CSSParserValue*);
    PassRefPtr<CSSPrimitiveValue> createPrimitiveStringValue(CSSParserValue*);

    bool validWidthOrHeight(CSSParserValue*);

    PassRefPtr<CSSBasicShape> parseInsetRoundedCorners(PassRefPtr<CSSBasicShapeInset>, CSSParserValueList*);

    class ImplicitScope {
        STACK_ALLOCATED();
        WTF_MAKE_NONCOPYABLE(ImplicitScope);
    public:
        ImplicitScope(CSSPropertyParser* parser)
            : m_parser(parser)
        {
            m_parser->m_implicitShorthand = true;
        }

        ~ImplicitScope()
        {
            m_parser->m_implicitShorthand = false;
        }

    private:
        CSSPropertyParser* m_parser;
    };

    class ShorthandScope {
        STACK_ALLOCATED();
    public:
        ShorthandScope(CSSPropertyParser* parser, CSSPropertyID propId) : m_parser(parser)
        {
            if (!(m_parser->m_inParseShorthand++))
                m_parser->m_currentShorthand = propId;
        }
        ~ShorthandScope()
        {
            if (!(--m_parser->m_inParseShorthand))
                m_parser->m_currentShorthand = CSSPropertyInvalid;
        }

    private:
        CSSPropertyParser* m_parser;
    };

    enum ReleaseParsedCalcValueCondition {
        ReleaseParsedCalcValue,
        DoNotReleaseParsedCalcValue
    };

    enum Units {
        FUnknown = 0x0000,
        FInteger = 0x0001,
        FNumber = 0x0002, // Real Numbers
        FPercent = 0x0004,
        FLength = 0x0008,
        FAngle = 0x0010,
        FTime = 0x0020,
        FFrequency = 0x0040,
        FPositiveInteger = 0x0080,
        FRelative = 0x0100,
        FResolution = 0x0200,
        FNonNeg = 0x0400
    };

    friend inline Units operator|(Units a, Units b)
    {
        return static_cast<Units>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }

    bool validCalculationUnit(CSSParserValue*, Units, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue);

    bool shouldAcceptUnitLessValues(CSSParserValue*, Units, CSSParserMode);

    inline bool validUnit(CSSParserValue* value, Units unitflags, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue) { return validUnit(value, unitflags, m_context.mode(), releaseCalc); }
    bool validUnit(CSSParserValue*, Units, CSSParserMode, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue);

    bool parseBorderImageQuad(Units, RefPtr<CSSPrimitiveValue>&);
    int colorIntFromValue(CSSParserValue*);
    bool isCalculation(CSSParserValue*);

private:
    // Inputs:
    CSSParserValueList* m_valueList;
    const CSSParserContext& m_context;
    const bool m_inViewport;

    // Outputs:
    Vector<CSSProperty, 256>& m_parsedProperties;
    CSSRuleSourceData::Type m_ruleType;

    // Locals during parsing:
    int m_inParseShorthand;
    CSSPropertyID m_currentShorthand;
    bool m_implicitShorthand;
    RefPtr<CSSCalcValue> m_parsedCalculation;

    // FIXME: There is probably a small set of APIs we could expose for these
    // classes w/o needing to make them friends.
    friend class ShadowParseContext;
    friend class BorderImageParseContext;
    friend class BorderImageSliceParseContext;
    friend class BorderImageQuadParseContext;
    friend class TransformOperationInfo;
    friend PassRefPtr<CSSPrimitiveValue> parseGradientColorOrKeyword(CSSPropertyParser*, CSSParserValue*);
};

CSSPropertyID cssPropertyID(const CSSParserString&);
CSSPropertyID cssPropertyID(const String&);
CSSValueID cssValueKeywordID(const CSSParserString&);

bool isKeywordPropertyID(CSSPropertyID);
bool isValidKeywordPropertyAndValue(CSSPropertyID, CSSValueID, const CSSParserContext&);

} // namespace blink

#endif  // SKY_ENGINE_CORE_CSS_PARSER_CSSPROPERTYPARSER_H_
