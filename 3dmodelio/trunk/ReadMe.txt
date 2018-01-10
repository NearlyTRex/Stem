Separate models for each format, with parsing details in them; simpler unified model objects independent of format implementation details; utility functions for converting between the two types


DATA MODEL: Hierarchical or flat? Lean toward hierarchical for easier runtime usage.
Input only. Feel free to be destructive with loading. Keep only relevant data and discard the rest. Reorganize it in a way that makes sense for runtime usage.

RUNTIME USAGE NEEDS:
- Identify meshes in scene (read names, keep list)
- Apply and modulate skeletal animations (might want IK or limb collision detection/hit testing); animation data for both CPU and GPU usage
- Render meshes with materials, textures, skeletal deformation, morph deformation, transformation (GPU should be doing all heavy lifting, including animation)
- Query mesh for collision detection and hit testing
- Initialize CollisionStaticTrimesh struct
- Apply transformations
- Play and query animation sequences
- Dispatch events at specific points in animation
 - Animation playback system probably separate from raw data representation


Export model as header suitable for creating StaticMeshModel
- glMapBuffer(GL_READ_ONLY) to pull data from GPU

TASKS:
- Load and render an animated model
- Figure out materials and lighting


Need custom format. glTF importer would be nice, but not a top priority. Doesn't export animation markers, separate curves for x/y/z, limited material properties. Maybe could extend exporter?

Tricky stuff: Managing armatures and animations shared between multiple SceneModels. Use IDs/names to bind instead of pointers?


ORDER:
- Data structure
- Import
- Export
- Normal mapping
- Materials/lighting?
- Animations