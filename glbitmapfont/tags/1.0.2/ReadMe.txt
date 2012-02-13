GLBitmapFont is a GLFont implementation that draws bitmapped text from an OpenGL texture atlas. Only printable ASCII characters are currently supported. Pairs of characters can be kerned.

The provided drawString() function has a few notable side effects and restrictions. Text is drawn using vertex arrays without making assumptions about which arrays are enabled, so GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY are enabled at the beginning of the function and disabled at the end. When drawing with OpenGL ES2, you must have a shader active which has 4 floats for vertex position in attribute position 0, and 2 floats for texture coordinates in attribute position 1; correspondingly, attribute arrays 0 and 1 will be disabled at the end of the function. Additionally, drawString uses GLTexture's activate() and deactivate(), which have side effects of their own; see the gltexture ReadMe.txt for more information. A future version of glbitmapfont will provide a mechanism for drawing without these side effects or restrictions.

GLBitmapFont serialization schema:

structure "glbitmapfont" {
	uint16 "format_version" // GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION
	string "texture_name"
	dictionary "characters"[95] {
		structure " " {
			float "glyph_offset"
			float "glyph_width"
			float "advance"
			array "texture_bounds"[4] {
				float // left
				float // right
				float // bottom
				float // top
			}
			dictionary "kerning_table" {
				optional float " "
				optional float "!"
				...
				optional float "~"
			}
		}
		structure "!" {
			...
		}
		...
		structure "~" {
			...
		}
	}
}
