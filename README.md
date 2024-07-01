# Graphics System
 
The following document specifies the data structures, functions, and functionality for a basic 3D rendering system using C. The data types within the system include the following.
- Pixel - data structure for holding image data for a single point
- Image - data structure for holding the data for a single image
- Z-buffer - a depth image, usually part of the Image data structure
- Point - a 2D or 3D location in model space
- Line - a line segment in 2D or 3D model space
- Circle - a 2D circle
- Ellipse - a 2D ellipse
- Polygon - a closed shape consisting of line segments
- Polyline - a sequence of connected line segments
- Color - a representation of light energy
- Light - a light source
- Vector - a direction in 2D or 3D model space
- Matrix - a 2D or 3D transformation matrix
- Element - a element of a model of a scene
- Module - a collection of elements
- View2D - information required for a 2D view of a scene
- ViewPerspective - information required for a perspective view of a scene
- ViewOrthographic - information required for an orthographic view of a scene