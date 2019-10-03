textures/decker/decker_tele2
{
	qer_editorimage textures/decker/decker_tele2.tga
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	cull disable
	tessSize 128
	q3map_surfacelight 3000
	q3map_lightimage textures/acid/4_white.tga

	{
		map textures/effects/envmap.tga
		rgbGen identity
		tcGen environment
	}
	{
		map textures/decker/decker_tele2.tga
		blendfunc blend
		rgbGen identity
		tcMod turb 0 0.3 0 0.2
	}
}
