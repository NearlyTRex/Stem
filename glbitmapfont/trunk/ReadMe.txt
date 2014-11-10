GLBitmapFont is a GLFont implementation that draws bitmapped text from an OpenGL texture atlas. Only printable ASCII characters are currently supported. Pairs of characters can be kerned.

Current GLBitmapFont serialization schema:

structure "glbitmapfont" {
	uint16 "format_version" // GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION
	string "atlas_name"
	dictionary "characters"[95] {
		structure " " {
			float "glyph_offset"
			float "glyph_width"
			float "advance"
			string "atlas_key"
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



GLBitmapFont serialization schema version 1:

structure "glbitmapfont" {
	uint16 "format_version" // 1
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
