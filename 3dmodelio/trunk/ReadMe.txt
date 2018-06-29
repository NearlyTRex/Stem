3DModelIO provides supporting data models and I/O code for glgraphics. Currently, only obj and stem3d files can be loaded, and only stem3d files can be saved.

armatureName, materialName, colorMapName, normalMapName, and meshName should all be file names in the directory next to the file containing them, or elsewhere in search paths

TODO: More info here

Note: MeshData packs vertices and indexes in little endian format only

TASKS:
- Why doesn't my model render the way Blender does? Colors are darker, light seems to react differently
 - Eyelids are all messed up. Normals pointing too downward?
  - Subdivided to fix, but understanding would be nice
 - Horn normals seem weird, but don't understand why
- Unit tests might be nice?
- Export options for io_scene_stem3d:
 - Binary format boolean
 - Mirror tangents boolean
 - Uniq vertices boolean? (Would have to call meshutil; Python too slow)
 - Create stem3d directory boolean
 - Replace files boolean
 - Select objects to export
- Improve bump2normal:
 - Output doesn't quite match Blender bump mapping. Figure out why.
 - Strength option might not make sense; why have to multiply by 32?
 - Convolution filter?
- Animation
 - May need IK constraints to keep feet on ground, possibly even at runtime
- Export SceneLayout


Animation:

Bone constraint panel (in rightmost pane)
Add IK constraint to target bone
Chain length (2?)
May need extra bones in armature for IK chain; naming convention to skip on export? ("NOEXPORT" anywhere in name?)