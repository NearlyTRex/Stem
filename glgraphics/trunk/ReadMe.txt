TODO: Update everything below, with descriptions for each class
GLGraphics provides some miscellaneous general-purpose OpenGL utility code. Included modules:

	* GLGraphics defines GLGraphics_init, which is to be called at startup (by the shell, if applicable) to initialize glew (if applicable) and define the OpenGL API version, which can later be retrieved with GLGraphics_getOpenGLAPIVersion.

	* GLIncludes.h is a convenience header that #includes the appropriate OpenGL headers for the target platform.

	* GLSLShader provides simplified vertex and fragment shader setup.

	* GLUtilities provides OpenGL utility functions for translating between string representations and GLenum representations of OpenGL constants.




SCRATCHPAD

Does Renderer even do 2D? Maybe just 3D and leave 2D to something else


[17:38]  <ThemsAllTook> Say I'm drawing a room, and a character walking from one side of it to the other. The room is just a static pile of vertices with a material or something; simple enough.
[17:38]  <ThemsAllTook> The character includes some animation data that needs to be passed on to the vertex shader, and some sort of extra animation state information needs to get there too somehow.
[17:38]  <ThemsAllTook> Does the renderer have a concept of animation data? How does it know which shader to use and how to shuffle the data around?
[17:39]  <KatieHuber> just a flag on the model to say "animated" or not
[17:40]  <KatieHuber> (animated models will have different vertex structure from static ones anyway, so always a different shader)
[17:40]  <KatieHuber> the game will submit "draw this object with this model and this state" for every object
[17:40]  <KatieHuber> for static objects, "state" is likely just a xform matrix
[17:40]  <KatieHuber> for animated, the full matrix palette, probably
[17:41]  <ThemsAllTook> OK, great, this is starting to make some sense
[17:41]  <KatieHuber> or it could just be an animation name and time and the renderer could calculate the palette
[17:43]  <ThemsAllTook> I think the trap I keep falling into is imagining that I'm going to want to render an arbitrary number of things in arbitrarily different ways, when I can probably just constrain myself to a few core concepts (static geometry, animated geometry (using just one specific type of animation), particles, etc.) and have my needs covered
[17:44]  <ThemsAllTook> It's just really hard for me to coalesce that list of things in my head. This helps a lot.


Mesh \
	Vertices \
		Position
		Normal
		Tangent
		Color
		Texcoords
		Bone IDs (4)
		Bone weights (4)
Armature \
	Bone hierarchy \
		Bone ID
		Transform relative to parent
Morph \
	Vertices \
		Position offset
		Normal
		Tangent
Animation \
	Armature reference
	Morph reference
	Frame rate
	Frames \
		Bone transforms \
			Bone ID
			Transformation matrix
		Morph weights \
			Morph mesh ID (?)
			Weight
AnimationState \
	Armature reference
	Bone transform list
	Morph reference list
	Morph weight list
MISSING: Animation collection, AnimationStateController or equivalent, animation transitions

Shape keys/morph targets are pretty tricky. Ideas:
- Organize vertices into groups which own morph sets, up to some limit in the vertex shader. Different glDrawElements calls for each group.
* Calculate all morphs in software, and resubmit vertex data every frame for animations that have morphs (only if dirty?).
- Limit entire mesh to some number of morphs, and encode all morph offsets into static vertex data with uniforms for weights.
- Encode vertex data as normal, but calculate offsets for everything at once in software and pass those in some other way. Sampler?
 - This is essentially no better than resubmitting vertex data every frame, since it contains a full list of positions/offsets anyway

Constraints and control objects are Blender-only. Exported animations can be flattened.

MATERIAL STUFF
- Color map (two layers?)
- Diffuse/specular/reflection map
- Normal map
- Displacement map
- Subsurface scattering (research needed)
- PBR
 - Metalness
 - Roughness
 - AO



IMMEDIATE: Model loader turns model into header

TODO:
- VAO binding in Renderable
- Controller object with graph logic?
- IK
- Advanced materials
- Lighting
- Morphing? (maybe better to do without)
- Callback renderable type
- Additional camera types (camera controller with interpolation)
- Depth sorting/translucency
- Fog
- Shadows
- SSAO
- Fluid simulation
- Cloth simulation