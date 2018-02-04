3DModelIO provides supporting data models and I/O code for glgraphics. Currently, only obj and stem3d files can be loaded, and only stem3d files can be saved.

armatureName, materialName, colorMapName, normalMapName, and meshName should all be file names in the directory next to the file containing them, or elsewhere in search paths

TODO: More info here

Note: MeshData packs vertices and indexes in little endian format only

TASKS:
- Export armatures
- Unit tests might be nice?
- Export options for io_scene_stem3d:
 - Binary format
 - Mirror tangents
 - Uniq vertices? (Would have to call meshutil; Python too slow)
 - Export SceneLayout, texture PNGs, material, armature
- Improve bump2normal:
 - Output doesn't quite match Blender bump mapping. Figure out why.
 - Strength option might not make sense; why have to multiply by 32?
 - Convolution filter?
- Animation
