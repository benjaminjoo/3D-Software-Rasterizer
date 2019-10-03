textures/decker/decker_tele
{
	qer_editorimage textures/decker/decker_tele.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nomarks
	cull disable
	tessSize 128
	q3map_surfacelight 100

	{
		map textures/effects/envmap.tga
		rgbGen identity
		tcGen environment
	}
	{
		map textures/decker/decker_tele.tga
		blendfunc blend
		rgbGen identity
		tcMod turb 0 0.3 0 0.2
	}
}
