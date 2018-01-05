TODO: Update everything below, with descriptions for each class
GLGraphics provides some miscellaneous general-purpose OpenGL utility code. Included modules:

	* GLGraphics defines GLGraphics_init, which is to be called at startup (by the shell, if applicable) to initialize glew (if applicable) and define the OpenGL API version, which can later be retrieved with GLGraphics_getOpenGLAPIVersion.

	* GLIncludes.h is a convenience header that #includes the appropriate OpenGL headers for the target platform.

	* GLSLShader provides simplified vertex and fragment shader setup.

	* GLUtilities provides OpenGL utility functions for translating between string representations and GLenum representations of OpenGL constants.




SCRATCHPAD

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
- Specular map
- Normal map
- Displacement map
- Occlusion map
- Subsurface scattering (research needed)
- PBR
 - Metalness
 - Roughness
 - AO


TODO:
- Camera work:
 - Camera interpolation (controller object interpolates between different camera objects)
 - Other cameras: First person, follow behind/ahead
- Animation controller object with graph logic? Also event dispatching
- IK (https://www.webcitation.org/60uCFHqC2?url=http://freespace.virgin.net/hugo.elias/models/m_ik2.htm)
- Advanced materials
- Better lighting
- Morphing? (maybe better to do without)
- Callback renderable type
- Additional camera types (camera controller with interpolation)
- Depth sorting/translucency
- Culling
- Fog
- Shadows
- SSAO
- Fluid simulation
- Cloth simulation
- VR


SceneModel needs:
- Load from file
- Write to file
- Initialize from obj, glTF, possibly other formats
- Create MeshRenderable, Material, Armature, Animation
- Create CollisionTrimesh
- Merge or cross reference multiple SceneModels

SceneModel lives in glgraphics
Classes that can initialize from SceneModel data have their own initialization functions that pull from SceneModel; SceneModel doesn't push to them
ResourceManager needs to be rewritten to work with SceneModel needs, plus audio, level data, etc.
- Keep data around for a while when reference count drops to 0. Purge when memory pressure is detected, or some amount of time has elapsed.
- To measure memory pressure, ResourceManager needs to know how large each item is
- Background thread could perform unloading, kicked off by next resource request for old-enough 0-reference items
Blob mechanism should maybe be its own thing? Useful for other stuff

New idea: Granular export. Each type of data has its own format and data models.
- Armature
- Animation
- Mesh
- Texture
- Layout (includes light data?)

SceneLayout becomes new StemObject
MeshData becomes new StemObject (or just struct?), and is the basis for instantiating MeshRenderable and CollisionTrimesh
TextureData same as above
Material, Armature, and Animation need to be compatible with above organization
May need ReferenceCount logic in utilities


DATA STRUCTURE STATUS
- Animation: √
- Armature: √
- Material: X
 - Work out how to reference textures
 - Light interaction properties (color, specularity, emissiveness)
- Mesh: X
 - MeshRenderable not suitable for serialization. MeshData might be almost? How to create MeshRenderable from MeshData?
- SceneLayout: +
 - Maybe good enough. Open question: How are meshes found by name? Who keeps list?
 - Lights need refinement. Possibly additional types, attenuation, other properties
- Texture: ? (is this a different thing than Material?)