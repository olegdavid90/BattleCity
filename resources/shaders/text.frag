uniform sampler2D texture;
uniform vec4      fillColor;

void main()
{
	gl_FragColor = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	
	if (gl_FragColor.rgb == vec3(0.0))
	{
		discard;
	}
	if (gl_FragColor.rgb == vec3(1.0))
	{
		gl_FragColor = fillColor;
	}
}
