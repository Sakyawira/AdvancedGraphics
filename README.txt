Features:
1. Star Model with Geometry Model
2. LODed Quad with Tesselation.
3. Non-LODed Quad with Tesselation Model ( 3 inner subdivisions in x and y and 5 outer subdivisions)
4. Terrain Generated using Heightmap.
5. A Cube that follows the Terrain height.
6. Framebuffer.
7. Blur,scanlines and chromatic aberration post processing fx added to the framebuffer texture.
8. Generated grass using geometry shader.
9. Grass are mapped using height of the terrain.

Controls:
1. Use WASD to move the red cube around.
2. Click 'R' once to go into free moving camera mode. The cube will now follow the player's cursor, it will still follow the terrain.
3. Click 'Space' to change polygon view mode.
4. While in Free Moving Camera and Wireframe mode, go closer and further from the middle yellow quad, you should be able to see the LOD changes. 