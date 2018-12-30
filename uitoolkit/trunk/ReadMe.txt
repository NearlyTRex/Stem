Broad goals:
- Assist with basic layout
- Provide a few widgets and basic facilities for building custom ones
- Assist with drawing
- Accept mouse, keyboard, and gamepad inputs and dispatch high-level events

REDEFINE: Only handle layout and event flow. Ignore everything below.
 - UIInputController is event dump target which hit tests and dispatches to appropriate widget in flat list
 - UILayout is list of UIElements, with enumeration methods?
 - UINavigationController provides directional input functions (menuUp, menuDown, menuLeft, etc.) and maps to focusing specific elements
 - What about drawing? Any support at all? UILayout has getAllVertices method?
 - Maybe a few standard widgets of some sort? Button, at least?

Needs:
- Root container object (UICanvas?)
- Subcontainers
- Anchor to left/right/bottom/top/center
- Mouse event detection
- Keyboard navigation
- Widgets:
 - (high) Button
 - (high) Text field
 - (high) Check box
 - (high) Radio button
 - (high) Slider
 - (high) Custom
 - (medium) Stepper
 - (medium) Edit text
 - (medium) File path picker
 - (low) Pop-up menu
 - (low) List
 - (low) Color picker
 - (low) Date/time
- (low) Scrolling container

Event flow:
MOUSE
 - Top-level container receives window coordinates, knows its coordinate system, and transforms
 - Each container performs hit testing and calls event handlers on children as appropriate (?)
GAMEPAD
 - Expect target to have used InputController to transform into menuLeft, menuRight, menuUp, menuDown, menuOK
  - What about menuCancel? What about other buttons?
KEYBOARD
 - Expect InputController as above, but also need text input
 - Tab/reverse tab; need deep enumeration


Concepts:
- Input: Mouse (hit test, down/drag/up), keyboard/gamepad (menuUp/Left/Right/Down, accept, cancel)
- Appearance: Texture atlas, dimensions, slices for all graphical elements; bitmap font; metrics?
- Layout: Container, anchor zone, offset (container and layout can be one and the same?)

TODO:
- Create UICheckBox, UIRadioGroup, UIEditText, UISlider, UIPopUpMenu, UIScrollingContainer
- Figure out UIContainer subclasses
- Come up with a way to make text rendering work in both pixel-exact and non-pixel-exact modes (appearance parameter?)
- Make UIContainer_removeElement() safe for iteration
