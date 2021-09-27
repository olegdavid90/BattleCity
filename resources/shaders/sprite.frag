uniform sampler2D texture;

void main()
{
	gl_FragColor = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	
	if (gl_FragColor.rgb == vec3(0.0))
	{
		discard;
	}
}
