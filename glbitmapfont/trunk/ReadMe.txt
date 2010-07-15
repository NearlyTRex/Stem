GLBitmapFont serialization schema:

structure "glbitmapfont" {
	string "texture_name"
	dictionary "characters"[95] {
		structure " " {
			float "glyph_offset"
			float "glyph_width"
			float "advance"
			array "texture_bounds"[4] {
				float
				float
				float
				float
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