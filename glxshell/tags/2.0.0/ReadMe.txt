GLXShell is an implementation of a stem shell for Linux using glX. It is a feature complete, production-ready shell, with the following exceptions:

Shell_getDisplayCount() always returns 1
Shell_getDisplayIndexFromWindow() always returns 0
Shell_getDisplayBounds() ignores its displayIndex parameter and may return unexpected width/height values for if multiple displays are attached
Shell_enterFullScreen() ignores its displayIndex parameter and will enter full screen mode on the display containing the main window