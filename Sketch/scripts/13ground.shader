textures/13ground/metal14_1_trans
{
	qer_editorimage textures/base_support/metal14_1.tga
	surfaceparm nonsolid
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_support/metal14_1.tga
		blendfunc filter
		rgbGen identity
	}
}

textures/13ground/bluemetal2_trans
{
	qer_editorimage textures/base_wall/bluemetal2.tga
	surfaceparm nonsolid
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_wall/bluemetal2.tga
		blendfunc filter
		rgbGen identity
	}
}

textures/13ground/bouncepad_xarch
{
	surfaceparm nodamage
	q3map_lightimage textures/sfx/jumppadsmall.tga	
	q3map_surfacelight 400
	{
		map textures/13ground/bouncepad_xarch.tga
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/sfx/bouncepad01b_layer1.tga
		blendfunc gl_one gl_one
		rgbGen wave sin .5 .5 0 1.5	
	}
	{
		clampmap textures/sfx/jumppadsmall.tga
		blendfunc gl_one gl_one
		tcMod stretch sin 1.2 .8 0 1.5
		rgbGen wave square .5 .5 .25 1.5
	}
}

textures/13ground/pewter_shiney_red
{
	qer_editorimage textures/base_trim/pewter_shiney.tga
	{
		map textures/13ground/tinfx_red.tga
		rgbGen identity
		tcGen environment 
	}
	{
		map textures/base_trim/pewter_shiney.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc gl_dst_color gl_one_minus_dst_alpha
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/pewter_shiney_blue
{
	qer_editorimage textures/base_trim/pewter_shiney.tga
	{
		map textures/13ground/tinfx_blue.tga
		rgbGen identity
		tcGen environment 
	}
	{
		map textures/base_trim/pewter_shiney.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc gl_dst_color gl_one_minus_dst_alpha
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/evil_window_red
{
	qer_editorimage textures/gothic_block/windowevil2c_killblock.tga
	q3map_lightimage textures/gothic_block/evil2cglow.tga
	q3map_lightsubdivide 128
	q3map_surfacelight 200
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/gothic_block/windowevil2c_killblock.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/gothic_block/evil2cglow.tga
		blendfunc add
	}
}

textures/13ground/evil_window_blue
{
	qer_editorimage textures/gothic_block/windowevil2c_killblock.tga
	q3map_lightimage textures/13ground/evilglow_blue.tga
	q3map_lightsubdivide 128
	q3map_surfacelight 200
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/gothic_block/windowevil2c_killblock.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/13ground/evilglow_blue.tga
		blendfunc add
	}
}

textures/13ground/atech1_red
{
	qer_editorimage textures/13ground/atech1_red.tga
	q3map_lightimage textures/gothic_block/evil2cglow.tga
	q3map_lightsubdivide 128
	q3map_surfacelight 100
	{
		map textures/sfx/zap_scroll2.tga
		rgbGen identity
		tcMod scale 2 0.4
		tcMod scroll 6 0.6
	}
	{
		map textures/13ground/atech1_red.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/atech1_blue
{
	qer_editorimage textures/13ground/atech1_blue.tga
	q3map_lightimage textures/13ground/evilglow_blue.tga
	q3map_lightsubdivide 128
	q3map_surfacelight 100
	{
		map textures/sfx/hologirl.tga
		rgbGen identity
		tcMod scale 2 0.4
		tcMod scroll 6 0.6
	}
	{
		map textures/13ground/atech1_blue.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/flagbase_red
{
	qer_editorimage textures/gothic_floor/center2trn.tga
	{
		map textures/tim/swirl_red.tga
		rgbGen identity
		tcMod rotate 300
		tcMod stretch sin 0.8 0.2 0 9.7 
	}
	{
		map textures/gothic_floor/center2trn.tga
		blendfunc blend
		rgbGen identity
		tcMod rotate 30
		tcMod stretch sin 0.8 0.2 0 0.2 
	}
	{
		map textures/gothic_floor/center2trn.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc gl_dst_color gl_one_minus_dst_alpha
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/flagbase_blue
{
	qer_editorimage textures/gothic_floor/center2trn.tga
	{
		map textures/tim/swirl_blue.tga
		rgbGen identity
		tcMod rotate 300
		tcMod stretch sin 0.8 0.2 0 9.7 
	}
	{
		map textures/gothic_floor/center2trn.tga
		blendfunc blend
		rgbGen identity
		tcMod rotate 30
		tcMod stretch sin 0.8 0.2 0 0.2 
	}
	{
		map textures/gothic_floor/center2trn.tga
		blendfunc blend
		rgbGen identity
	}
	{
		map $lightmap 
		blendfunc gl_dst_color gl_one_minus_dst_alpha
		rgbGen identity
		tcGen lightmap 
	}
}

textures/13ground/ta_techspawn_red
{
	qer_editorimage textures/13ground/tech.tga
	surfaceparm nonsolid
	surfaceparm trans
	{
		clampmap textures/13ground/tech_fx_red.tga
		rgbGen identity
		tcMod rotate 222
	}
	{
		clampmap textures/13ground/tech.tga
		blendfunc blend
		rgbGen Vertex
	}
}

textures/13ground/ta_techspawn_blue
{
	qer_editorimage textures/13ground/tech.tga
	surfaceparm nonsolid
	surfaceparm trans
	{
		clampmap textures/13ground/tech_fx_blue.tga
		rgbGen identity
		tcMod rotate 222
	}
	{
		clampmap textures/13ground/tech.tga
		blendfunc gl_one gl_one_minus_src_alpha
		rgbGen Vertex
	}
}

textures/13ground/grate
{
	qer_editorimage textures/base_floor/pjgrate2.tga
	surfaceparm metalsteps
	cull disable
	nopicmip
	{
		map textures/base_floor/pjgrate2.tga
		rgbGen identity
		tcMod scale 2 2
		depthWrite
		alphaFunc GE128
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
		tcGen lightmap 
		depthFunc equal
	}
}

textures/13ground/ceil1_red_4k
{
	qer_editorimage textures/base_light/ceil1_22a.tga
	surfaceparm nomarks
	q3map_surfacelight 4000
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_light/ceil1_22a.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/base_light/ceil1_22a.blend.tga
		blendfunc add
	}
}

textures/13ground/ceil1_blue_4k
{
	qer_editorimage textures/base_light/ceil1_30.tga
	surfaceparm nomarks
	q3map_surfacelight 4000
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_light/ceil1_30.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/base_light/ceil1_30.blend.tga
		blendfunc add
	}
}

textures/13ground/light1red_10k
{
	qer_editorimage textures/base_light/light1red.tga
	q3map_lightimage textures/base_light/light1red.blend.tga
	surfaceparm nomarks
	q3map_surfacelight 10000
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_light/light1red.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/base_light/light1red.blend.tga
		blendfunc add
	}
}

textures/13ground/light1blue_10k
{
	qer_editorimage textures/base_light/light1blue.tga
	q3map_lightimage textures/base_light/light1blue.blend.tga
	surfaceparm nomarks
	q3map_surfacelight 10000
	{
		map $lightmap 
		rgbGen identity
		tcGen lightmap 
	}
	{
		map textures/base_light/light1blue.tga
		blendfunc filter
		rgbGen identity
	}
	{
		map textures/base_light/light1blue.blend.tga
		blendfunc add
	}
}

textures/13ground/beam_red
{
	qer_editorimage textures/13ground/beam_red.tga
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	nopicmip
	qer_trans 0.6
	{
		map textures/13ground/beam_red.tga
		blendfunc add
		tcMod scroll 0.3 0
	}
}

textures/13ground/beam_blue
{
	qer_editorimage textures/13ground/beam_blue.tga
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	nopicmip
	qer_trans 0.6
	{
		map textures/13ground/beam_blue.tga
		blendfunc add
		tcMod scroll 0.3 0
	}
}

textures/13ground/lavahell_2k
{
	qer_editorimage textures/liquids/lavahell.tga
	surfaceparm lava
	surfaceparm noimpact
	surfaceparm nolightmap
	cull disable
	deformVertexes wave 100 sin 3 2 0.1 0.1 
	q3map_lightsubdivide 32
	q3map_surfacelight 2000
	q3map_globaltexture
	{
		map textures/liquids/lavahell.tga
		tcMod turb 0 0.2 0 0.1
	}
}

textures/13ground/lavablue_2k
{
	qer_editorimage textures/liquids/pool2.tga
	surfaceparm lava
	surfaceparm noimpact
	surfaceparm nolightmap
	cull disable
	deformVertexes wave 100 sin 3 2 0.1 0.1 
	q3map_lightsubdivide 32
	q3map_surfacelight 2000
	q3map_globaltexture
	{
		map textures/liquids/pool2.tga
		tcMod turb 0 0.2 0 0.1
	}
}

textures/13ground/sky
{
	qer_editorimage textures/skies/inteldimredclouds.tga
	q3map_lightimage textures/skies/toxicsky.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightsubdivide 128
	q3map_surfacelight 800
	q3map_sun 1 1 0.5 30 0 90
	q3map_sun 1 1 0.5 10 45 89
	q3map_sun 1 1 0.5 10 135 89
	q3map_sun 1 1 0.5 10 225 89
	q3map_sun 1 1 0.5 10 315 89
	skyParms - 512 -
	{
		map textures/skies/inteldimclouds.tga
		tcMod scroll 0.1 0.1
		tcMod scale 3 2
		depthWrite
	}
	{
		map textures/skies/intelredclouds.tga
		blendfunc add
		tcMod scroll 0.05 0.05
		tcMod scale 3 3
	}
}

textures/13ground/sky_nolight
{
	qer_editorimage textures/skies/inteldimredclouds.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_sun 1 1 0.5 30 0 90
	q3map_sun 1 1 0.5 10 45 89
	q3map_sun 1 1 0.5 10 135 89
	q3map_sun 1 1 0.5 10 225 89
	q3map_sun 1 1 0.5 10 315 89
	skyParms - 512 -
	{
		map textures/skies/inteldimclouds.tga
		tcMod scroll 0.1 0.1
		tcMod scale 3 2
		depthWrite
	}
	{
		map textures/skies/intelredclouds.tga
		blendfunc add
		tcMod scroll 0.05 0.05
		tcMod scale 3 3
	}
}

textures/13ground/white
{
	surfaceparm nodlight
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nosteps
	{
		map textures/13ground/white.tga
		rgbGen identity
	}
}

textures/13ground/fog_red
{
	qer_editorimage textures/sfx/fog_red.tga
	surfaceparm fog
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	q3map_lightsubdivide 128
	q3map_surfacelight 300
	q3map_globaltexture
	fogparms ( 0.5 0.12 0.1 ) 256
	{
		map textures/liquids/kc_fogcloud3.tga
		blendfunc filter
		rgbGen identity
		tcMod scale -0.05 -0.05
		tcMod scroll 0.01 -0.01
	}
	{
		map textures/liquids/kc_fogcloud3.tga
		blendfunc filter
		rgbGen identity
		tcMod scale 0.05 0.05
		tcMod scroll 0.01 -0.01
	}
}

textures/13ground/fog_blue
{
	qer_editorimage textures/sfx/fog_blue.tga
	surfaceparm fog
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	q3map_lightsubdivide 128
	q3map_surfacelight 300
	q3map_globaltexture
	fogparms ( 0.1 0.12 0.5 ) 225
	{
		map textures/liquids/kc_fogcloud3.tga
		blendfunc filter
		rgbGen identity
		tcMod scale -0.05 -0.05
		tcMod scroll 0.01 -0.01
	}
	{
		map textures/liquids/kc_fogcloud3.tga
		blendfunc filter
		rgbGen identity
		tcMod scale 0.05 0.05
		tcMod scroll 0.01 -0.01
	}
}

models/mapobjects/13ground/banner_blue
{
	surfaceparm alphashadow
	surfaceparm nolightmap
	sort banner
	cull disable
	deformVertexes wave 100 sin 0 3 0 0.7 
	{
		map models/mapobjects/13ground/banner_blue.tga
		blendfunc blend
		rgbGen Vertex
		alphaFunc GE128
	}
}

