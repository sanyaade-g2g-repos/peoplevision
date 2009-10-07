/*
 *  ofxDspTypes.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_TYPES
#define OFX_DSP_TYPES

//	----------------------------------------------------------------------------------------------------

#include "ofMain.h"

//	----------------------------------------------------------------------------------------------------

#define OFXDSP_VERSION		"0.10"
#define OFXDSP_XML			"ofxDsp.xml"

#define OFXDSP_DENORMALS	1e-15f
#define OFXDSP_CHANNELS		2
#define OFXDSP_SAMPLERATE	44100.0f
#define OFXDSP_BUFFERSIZE	512
#define OFXDSP_MAXPARAM		2000

//	----------------------------------------------------------------------------------------------------

struct ofxDspParameter
{
	int				objectId;
	int				objectType;
	int				parameterId;
	float			value;
};

struct ofxDspParameterInfo
{
	string			name;
	int				unit;
	float			min;
	float			max;
	float			value;
	int				steps;
	const string*	strings;
};

struct ofxDspObjectInfo
{
	string						name;
	int							numberOfParameter;
	const ofxDspParameterInfo*	parameterInfo;
};

enum 
{
	kofxDsp_Bool,
	kofxDsp_Int,
	kofxDsp_Float,
};

enum 
{
	kofxDsp_ByPass,
	kofxDsp_Active,
};

//	----------------------------------------------------------------------------------------------------

enum 
{
	kofxDsp_Amplifier_Active,
	kofxDsp_Amplifier_Spread,
	kofxDsp_Amplifier_Panorama,
	kofxDsp_Amplifier_Level,
	kofxDsp_Amplifier_NumberOfParameter,
};

static const ofxDspParameterInfo kofxDsp_Amplifier_Info[] =
{
	{"Active",		kofxDsp_Bool,	0.0f,	1.0f,	kofxDsp_Active	, 0, NULL},
	{"Spread",		kofxDsp_Float,	0.0f,	1.0f,	0.0f			, 0, NULL},
	{"Panorama",	kofxDsp_Float,	0.0f,	1.0f,	0.5f			, 0, NULL},
	{"Level",		kofxDsp_Float,	0.0f,	1.0f,	1.0f			, 0, NULL},
};

//	----------------------------------------------------------------------------------------------------

enum 
{
	kofxDsp_Filter_Active,
	kofxDsp_Filter_Type,
	kofxDsp_Filter_Cutoff,
	kofxDsp_Filter_Resonance,
	kofxDsp_Filter_NumberOfParameter,
};

enum
{
	kofxDsp_Filter_LP12,
	kofxDsp_Filter_HP12,
	kofxDsp_Filter_BP12,
	kofxDsp_Filter_BR12,
	kofxDsp_Filter_PK12,
};

static const string kofxDsp_Filter_Types[] =
{
	"Lowpass",
	"Highpass",
	"Bandpass",
	"Bandreject",
	"Peak",
};

static const ofxDspParameterInfo kofxDsp_Filter_Info[] =
{
	{"Active",		kofxDsp_Bool,	0.0f,	1.0f,		kofxDsp_Active		, 0, NULL},
	{"Type",		kofxDsp_Int,	0.0f,	4.0f,		kofxDsp_Filter_LP12	, 5, &kofxDsp_Filter_Types[0]},
	{"Cutoff",		kofxDsp_Float,	40.0f,	22050.0f,	20000.0f			, 0, NULL},
	{"Resonance",	kofxDsp_Float,	0.0f,	1.0f,		0.0f				, 0, NULL},
};

//	----------------------------------------------------------------------------------------------------

enum 
{
	kofxDsp_Oscillator_Active,
	kofxDsp_Oscillator_Type,
	kofxDsp_Oscillator_Frequency,
	kofxDsp_Oscillator_Tuning,
	kofxDsp_Oscillator_NumberOfParameter,
};

enum
{
	kofxDsp_Oscillator_Sine,
	kofxDsp_Oscillator_Saw,
	kofxDsp_Oscillator_Rect,
	kofxDsp_Oscillator_Noise,
};

static const string kofxDsp_Oscillator_Types[] =
{
	"Sine",
	"Saw",
	"Rect",
	"Noise",
};

static const ofxDspParameterInfo kofxDsp_Oscillator_Info[] =
{
	{"Active",		kofxDsp_Bool,	0.0f,	1.0f,		kofxDsp_Active			, 0, NULL},
	{"Type",		kofxDsp_Int,	0.0f,	3.0f,		kofxDsp_Oscillator_Sine	, 4, &kofxDsp_Oscillator_Types[0]},
	{"Frequency",	kofxDsp_Float,	1.0f,	500.0f,		100.0f					, 0, NULL},
	{"Tuning",		kofxDsp_Float,	0.0f,	1.0f,		0.5f					, 0, NULL},
};

//	----------------------------------------------------------------------------------------------------

enum 
{
	kofxDsp_Shaper_Active,
	kofxDsp_Shaper_Type,
	kofxDsp_Shaper_Drive,
	kofxDsp_Shaper_Level,	
	kofxDsp_Shaper_NumberOfParameter,
};

enum
{
	kofxDsp_Shaper_Diode,
	kofxDsp_Shaper_Silicon,
};

static const string kofxDsp_Shaper_Types[] =
{
	"Diode",
	"Silicon",
};

static const ofxDspParameterInfo kofxDsp_Shaper_Info[] =
{
	{"Active",	kofxDsp_Bool,	0.0f,	1.0f,	kofxDsp_Active			, 0, NULL},
	{"Type",	kofxDsp_Int,	0.0f,	1.0f,	kofxDsp_Shaper_Diode	, 2, &kofxDsp_Shaper_Types[0]},
	{"Drive",	kofxDsp_Float,	0.0f,	1.0f,	0.5f					, 0, NULL},
	{"Level",	kofxDsp_Float,	0.0f,	1.0f,	0.5f					, 0, NULL},
};

//	----------------------------------------------------------------------------------------------------

enum 
{
	kofxDsp_Amplifier,
	kofxDsp_Filter,
	kofxDsp_Oscillator,
	kofxDsp_Shaper,
	kofxDsp_NumberOfObject,
};

static const ofxDspObjectInfo kofxDsp_Object_Info[] =
{
	{"Amplifier",	kofxDsp_Amplifier_NumberOfParameter,	&kofxDsp_Amplifier_Info[0]},
	{"Filter",		kofxDsp_Filter_NumberOfParameter,		&kofxDsp_Filter_Info[0]},
	{"Oscillator",	kofxDsp_Oscillator_NumberOfParameter,	&kofxDsp_Oscillator_Info[0]},
	{"Shaper",		kofxDsp_Shaper_NumberOfParameter,		&kofxDsp_Shaper_Info[0]},
};

//	----------------------------------------------------------------------------------------------------

static inline float hardClip(float x)
{
	float x1 = fabsf(x + 1.0f);
	float x2 = fabsf(x - 1.0f);
	
	return 0.5f * (x1 - x2);
}

static inline float fastTanh(float x)
{
	return	1.0f - 2.0f / (exp(x) + 1.0f);
}

static inline float halfDouble(float x)
{
	return pow(2.0f, (x - 0.5f) * 2.0f);
}

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
