/* 
 *  Filename:    HSL.h
 *  Description: HSL class implementation
 *               (adopted from http://www.alexkuhl.org/code/cpp/rgb_hsv_hsl.hpp)
 *  Copyright:   Julijan Šribar, 2012
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author(s) be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include "Windows.h"

template<typename T>
const T& Min3(const T& t1, const T& t2, const T& t3)
{
	return (t2 < t3) ? (t1 < t2 ? t1 : t2) : (t1 < t3 ? t1 : t3);
}

template<typename T>
const T& Max3(const T& t1, const T& t2, const T& t3)
{
	return (t2 > t3) ? (t1 > t2 ? t1 : t2) : (t1 > t3 ? t1 : t3);
}

class HSL
{
public:
	HSL(double hue, double saturation, double lightness) 
		: m_hue(hue)
		, m_saturation(saturation)
		, m_lightness(lightness)
	{ 
		ATLASSERT(hue >= 0. && hue < 360.);
		ATLASSERT(saturation >= 0. && saturation <= 1.);
		ATLASSERT(lightness >= 0. && lightness <= 1.);
	}

	HSL(COLORREF rgb) : m_hue(0.), m_saturation(0.), m_lightness(0.)
	{
		double red = GetRValue(rgb) / 255.;
		double green = GetGValue(rgb) / 255.;
		double blue = GetBValue(rgb) / 255.;
		double rgbMin = Min3(red, green, blue);
		double rgbMax = Max3(red, green, blue);
		double chroma = rgbMax - rgbMin;
		m_lightness = (rgbMin + rgbMax) / 2.;

		// saturation and hue are equal to 0
		if (chroma == 0)
			return;

		if (m_lightness <= 0.5)
			m_saturation = chroma / 2. / m_lightness;
		else
			m_saturation = chroma / (2. - 2. * m_lightness);

		// find dominant color
		if (rgbMax == red)
		{
			m_hue = 60. * (green - blue) / chroma;
			if (m_hue < 0)
				m_hue += 360;
		}
		else if (rgbMax == green)
			m_hue = 120. + 60. * (blue - red) / chroma;
		else // rgbMax == blue
			m_hue = 240. + 60. * (red - green) / chroma;
	}

	double GetHue() const { return m_hue; }
	double GetSaturation() const { return m_saturation; }
	double GetLightness() const { return m_lightness; }

	void SetHue(double hue)
	{
		ATLASSERT(hue >= 0.0 && hue <= 360.);
		m_hue = hue; 
	}
	
	void SetLightness(double lightness)
	{ 
		ATLASSERT(lightness >= 0.0 && lightness <= 1.0);
		m_lightness = lightness;
	}

	void SetSaturation(double saturation)
	{
		ATLASSERT(saturation >= 0.0 && saturation <= 1.0);
		m_saturation = saturation;
	}

	COLORREF GetRGB()
	{
		double q = (m_lightness < 0.5) ? m_lightness * (1. + m_saturation) : m_lightness + m_saturation - (m_lightness * m_saturation);
		double p = 2. * m_lightness - q;
		double h = m_hue / 360.;
		// t holds r, g, and b components
		double t[3] =	{ 
							h + 1. / 3.,
							h,
							h - 1. / 3.
						};
		for (int i = 0 ; i < 3 ; ++i)
		{
			if (t[i] < 0)
				t[i] += 1.;
			else if (t[i] > 1.)
				t[i] -= 1.;
			// calculate the color
			if (t[i] < 1. / 6.)
				t[i] = p + ((q - p) * t[i] * 6.);
			else if (t[i] < 0.5)
				t[i] = q;
			else if (t[i] < 2. / 3.)
				t[i] = p + (q - p) * (4. - 6. * t[i]);
			else
				t[i] = p;
		}
		ATLASSERT(t[0] >= 0 && t[0] <= 1.);
		int r = static_cast<int>(t[0] * 255. + 0.5);
		ATLASSERT(t[1] >= 0 && t[1] <= 1.);
		int g = static_cast<int>(t[1] * 255. + 0.5);
		ATLASSERT(t[2] >= 0 && t[2] <= 1.);
		int b = static_cast<int>(t[2] * 255. + 0.5);
		return RGB(r, g, b);
	}

private:
	double m_hue;
	double m_saturation;
	double m_lightness;
};
