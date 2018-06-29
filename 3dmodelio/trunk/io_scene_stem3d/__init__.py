import bpy
import bmesh
import os
import base64
import math
import mathutils
import mathutils.geometry
import struct
import re
import subprocess

from bpy.props import (StringProperty)
from bpy_extras.io_utils import (ExportHelper)

bl_info = {
	"name": "Stem3D exporter",
	"author": "Alex Diener",
	"blender": (2, 78, 0),
	"location": "File > Export",
	"description": "Export as Stem3D",
	"category": "Import-Export"
}

def escape_string(string):
	return re.sub(r'"', r'\"', string)

def swizzle_position(vector):
	return mathutils.Vector((vector[0], vector[2], -vector[1]))

def swizzle_tangent(vector):
	return mathutils.Vector((vector[0], vector[2], -vector[1], 1.0))

def swizzle_rotation(quaternion):
	return mathutils.Quaternion((quaternion[0], quaternion[1], quaternion[3], -quaternion[2]))

def get_primitives(blender_mesh, blender_vertex_groups, armature_object):
	vertices = []
	indexes = []
	
	bone_name_map = {}
	if armature_object is not None:
		for bone in armature_object.data.bones:
			bone_name_map[bone.name] = len(bone_name_map)
	
	if blender_mesh.uv_layers.active:
		blender_mesh.calc_tangents()
	
	index = 0
	for blender_polygon in blender_mesh.polygons:
		loop_index_list = []
		
		if len(blender_polygon.loop_indices) == 3:
			loop_index_list.extend(blender_polygon.loop_indices)
			
		elif len(blender_polygon.loop_indices) > 3:
			polyline = []
			for loop_index in blender_polygon.loop_indices:
				vertex_index = blender_mesh.loops[loop_index].vertex_index
				vertex = blender_mesh.vertices[vertex_index].co
				polyline.append(mathutils.Vector((vertex[0], vertex[1], vertex[2])))
			
			triangles = mathutils.geometry.tessellate_polygon((polyline,))
			
			for triangle in triangles:
				loop_index_list.append(blender_polygon.loop_indices[triangle[0]])
				loop_index_list.append(blender_polygon.loop_indices[triangle[2]])
				loop_index_list.append(blender_polygon.loop_indices[triangle[1]])
			
		else:
			continue
		
		face_normal = blender_polygon.normal
		face_tangent = mathutils.Vector((0.0, 0.0, 0.0))
		for loop_index in blender_polygon.loop_indices:
			vertex = blender_mesh.loops[loop_index]
			face_tangent += vertex.tangent
		face_tangent.normalize()
		
		flip_tangent = False
		for loop_index in blender_polygon.loop_indices:
			vertex_index = blender_mesh.loops[loop_index].vertex_index
			vertex = blender_mesh.vertices[vertex_index].co
			if vertex[0] < 0.0:
				flip_tangent = True
				break
		
		for loop_index in loop_index_list:
			vertex_index = blender_mesh.loops[loop_index].vertex_index
			vertex = blender_mesh.vertices[vertex_index]
			position = None
			normal = None
			tangent = None
			tex_coord = None
			color = None
			bones = None
			weights = None
			position = swizzle_position(vertex.co)
			if blender_polygon.use_smooth:
				normal = swizzle_position(vertex.normal)
				tangent = swizzle_tangent(blender_mesh.loops[loop_index].tangent)
			else:
				normal = swizzle_position(face_normal)
				tangent = swizzle_tangent(face_tangent)
			if flip_tangent:
				tangent[3] = -tangent[3]
			if blender_mesh.uv_layers.active:
				tex_coord = blender_mesh.uv_layers.active.data[loop_index].uv
			else:
				tex_coord = (0.0, 0.0)
			color = (1.0, 1.0, 1.0, 1.0)
			
			bones = []
			weights = []
			if vertex.groups is not None and armature_object is not None:
				for vertex_group in vertex.groups:
					if bone_name_map.get(blender_vertex_groups[vertex_group.group].name) is not None:
						bones.append(bone_name_map[blender_vertex_groups[vertex_group.group].name])
						weights.append(vertex_group.weight)
			
			if len(bones) == 0:
				bones.append(0)
				weights.append(1.0)
			while len(bones) < 4:
				bones.append(0)
				weights.append(0.0)
			
			vertices.append([position, tex_coord, normal, tangent, color, bones, weights])
			indexes.append(index)
			index += 1
	
	return vertices, indexes

def pack_vertices(vertices, write_ptnxcbw):
	vertices_packed = bytearray()
	vertex_size = 12 * 4
	for vertex in vertices:
		vertices_packed += struct.pack("<fff", vertex[0][0], vertex[0][1], vertex[0][2])
		vertices_packed += struct.pack("<ff", vertex[1][0], vertex[1][1])
		vertices_packed += struct.pack("<fff", vertex[2][0], vertex[2][1], vertex[2][2])
		vertices_packed += struct.pack("<ffff", vertex[3][0], vertex[3][1], vertex[3][2], vertex[3][3])
		vertices_packed += struct.pack("<ffff", vertex[4][0], vertex[4][1], vertex[4][2], vertex[4][3])
		if write_ptnxcbw:
			vertices_packed += struct.pack("<ffff", vertex[5][0], vertex[5][1], vertex[5][2], vertex[5][3])
			vertices_packed += struct.pack("<ffff", vertex[6][0], vertex[6][1], vertex[6][2], vertex[6][3])
	return vertices_packed

def pack_indexes(indexes):
	indexes_packed = bytearray()
	index_size = 4
	for index in indexes:
		indexes_packed += struct.pack("<I", index)
	return indexes_packed

def encode_base64(bytes):
	encoded_bytes = base64.encodestring(bytes)
	return encoded_bytes.replace(b"\n", b"")

def find_blender_object(blender_data):
	for blender_object in bpy.data.objects:
		if blender_object.data == blender_data:
			return blender_object
	return None

def find_first_blender_object_with_pose():
	for blender_object in bpy.data.objects:
		if blender_object.pose is not None:
			return blender_object
	return None

def format_json_float(value):
	return format(value, 'f')

def write_mesh(context, file_path, blender_mesh, binary_format):
	blender_object = find_blender_object(blender_mesh)
	
	blender_mesh_triangulated_copy = blender_mesh.copy()
	triangulated_mesh = bmesh.new()
	triangulated_mesh.from_mesh(blender_mesh_triangulated_copy)
	bmesh.ops.triangulate(triangulated_mesh, faces=triangulated_mesh.faces[:], quad_method=0, ngon_method=0)
	triangulated_mesh.to_mesh(blender_mesh_triangulated_copy)
	triangulated_mesh.free()
	
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"mesh\",\n\t\"name\": \"")
	file.write(escape_string(blender_mesh.name).encode())
	
	file.write(b"\",\n\t\"armature\": ")
	armature_object = blender_object.find_armature()
	if armature_object is not None:
		file.write(b"\"" + armature_object.data.name.encode() + b".armature\"")
	else:
		file.write(b"null")
	
	file.write(b",\n\t\"material\": ")
	if blender_object.active_material is not None:
		file.write(b"\"" + blender_object.active_material.name.encode() + b".material\"")
	else:
		file.write(b"null")
	
	vertices, indexes = get_primitives(blender_mesh_triangulated_copy, blender_object.vertex_groups, armature_object)
	
	file.write(b",\n\t\"vertices\": \"")
	file.write(encode_base64(pack_vertices(vertices, armature_object is not None)))
	file.write(b"\",\n\t\"indexes\": \"")
	file.write(encode_base64(pack_indexes(indexes)))
	file.write(b"\"\n}")
	file.close()
	
	bpy.data.meshes.remove(blender_mesh_triangulated_copy)

def write_image(context, file_path, blender_image):
	context.scene.render.image_settings.file_format = 'PNG'
	context.scene.render.image_settings.color_depth = '8'
	blender_image.save_render(file_path, context.scene)

def write_material(context, file_path, blender_material, binary_format):
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"material\",\n\t\"name\": \"")
	file.write(escape_string(blender_material.name).encode())
	file.write(b"\",\n\t\"color\": {\"red\": ")
	#TODO: Something seems wrong with these color calculations. Intensity-only for now.
	#file.write(format_json_float(blender_material.diffuse_color[0] * blender_material.diffuse_intensity).encode())
	file.write(format_json_float(blender_material.diffuse_intensity).encode())
	file.write(b", \"green\": ")
	#file.write(format_json_float(blender_material.diffuse_color[1] * blender_material.diffuse_intensity).encode())
	file.write(format_json_float(blender_material.diffuse_intensity).encode())
	file.write(b", \"blue\": ")
	#file.write(format_json_float(blender_material.diffuse_color[2] * blender_material.diffuse_intensity).encode())
	file.write(format_json_float(blender_material.diffuse_intensity).encode())
	file.write(b", \"alpha\": ")
	if blender_material.use_transparency:
		file.write(format_json_float(blender_material.alpha).encode())
	else:
		file.write(b"1.0")
	file.write(b"},\n\t\"specularity\": ")
	file.write(format_json_float(blender_material.specular_intensity).encode())
	file.write(b",\n\t\"shininess\": ")
	file.write(format_json_float(128.0 * (float(blender_material.specular_hardness) - 1.0) / 510.0).encode())
	file.write(b",\n\t\"emissiveness\": ")
	file.write(format_json_float(blender_material.emit).encode())
	file.write(b",\n\t\"textures\": {\n\t\t\"color_map\": {\n\t\t\t\"name\": ")
	
	color_map_name = None
	normal_map_name = None
	bump_map_name = None
	bump_strength = 0.0
	for blender_texture_slot in blender_material.texture_slots:
		if blender_texture_slot and blender_texture_slot.texture and blender_texture_slot.texture.type == 'IMAGE' and blender_texture_slot.texture.image is not None:
			texture_name = blender_texture_slot.texture.image.name + ".png"
			write_image(context, os.path.join(os.path.dirname(file_path), texture_name), blender_texture_slot.texture.image)
			
			if blender_texture_slot.use_map_color_diffuse:
				color_map_name = texture_name
			if blender_texture_slot.use_map_normal:
				if blender_texture_slot.texture.use_normal_map:
					normal_map_name = texture_name
				else:
					bump_map_name = texture_name
					bump_strength = blender_texture_slot.normal_factor
	
	if color_map_name is not None:
		file.write(b"\"" + color_map_name.encode() + b"\"")
	else:
		file.write(b"null")
	file.write(b",\n\t\t\t\"magnify_nearest\": false\n\t\t},\n\t\t\"normal_map\": {\n\t\t\t\"name\": ")
	if normal_map_name is not None:
		file.write(b"\"" + normal_map_name.encode() + b"\"")
	else:
		file.write(b"null")
	file.write(b",\n\t\t\t\"magnify_nearest\": false\n\t\t}\n\t}\n}")
	file.close()
	
	if bump_map_name is not None and normal_map_name is not None:
		normal_map_path = os.path.join(os.path.dirname(file_path), normal_map_name)
		bump_normal_map_path = os.path.join(os.path.dirname(file_path), os.path.splitext(bump_map_name)[0] + "_converted.png")
		bump_map_path = os.path.join(os.path.dirname(file_path), bump_map_name)
		bump2normal_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "bump2normal")
		subprocess.run([bump2normal_path, bump_map_path, bump_normal_map_path, str(bump_strength * 32)])
		subprocess.run([bump2normal_path, "--merge", bump_normal_map_path, normal_map_path, normal_map_path])
		os.unlink(bump_normal_map_path)
		os.unlink(bump_map_path)

def write_armature(context, file_path, blender_armature, binary_format):
	#Armature must be in edit mode to be able to retrieve edit_bones. Armature must be visible to have its mode changed.
	blender_object = find_blender_object(blender_armature)
	if blender_object is not None:
		last_active = bpy.context.scene.objects.active
		was_hidden = blender_object.hide
		last_mode = bpy.context.object.mode
		
		bpy.context.scene.objects.active = blender_object
		blender_object.hide = False
		bpy.ops.object.mode_set(mode='EDIT')
	
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"armature\",\n\t\"name\": \"")
	file.write(escape_string(blender_armature.name).encode())
	file.write(b"\",\n\t\"bones\": {")
	first_bone = True
	for bone in blender_armature.edit_bones:
		if first_bone:
			first_bone = False
		else:
			file.write(b",")
		file.write(b"\n\t\t\"" + bone.name.encode() + b"\": {")
		file.write(b"\n\t\t\t\"parent\": ")
		if bone.parent is not None:
			file.write(b"\"" + bone.parent.name.encode() + b"\"")
		else:
			file.write(b"null")
		file.write(b",\n\t\t\t\"position\": {\"x\": " + format_json_float(bone.head[0]).encode() + b", \"y\": " + format_json_float(bone.head[2]).encode() + b", \"z\": " + format_json_float(-bone.head[1]).encode() + b"},")
		file.write(b"\n\t\t\t\"endpoint\": {\"x\": " + format_json_float(bone.tail[0]).encode() + b", \"y\": " + format_json_float(bone.tail[2]).encode() + b", \"z\": " + format_json_float(-bone.tail[1]).encode() + b"},")
		offset, rotation, scale = bone.matrix.decompose()
		rotation = swizzle_rotation(rotation)
		file.write(b"\n\t\t\t\"base_orientation\": {\"x\": " + format_json_float(rotation[1]).encode() + b", \"y\": " + format_json_float(rotation[2]).encode() + b", \"z\": " + format_json_float(rotation[3]).encode() + b", \"w\": " + format_json_float(rotation[0]).encode() + b"}\n\t\t}")
	file.write(b"\n\t}\n}")
	file.close()
	
	#Restore armature properties if they were modified above.
	if blender_object is not None:
		blender_object.hide = was_hidden
		bpy.ops.object.mode_set(mode=last_mode)
		bpy.context.scene.objects.active = last_active

def read_pose_bone_state(pose_bone):
	armature_bone = pose_bone.bone
	#offset = mathutils.Vector((pose_bone.head[0] - armature_bone.head[0], pose_bone.head[1] - armature_bone.head[1], pose_bone.head[2] - armature_bone.head[2]))
	offset = mathutils.Vector((0, 0, 0))
	scale = mathutils.Vector((pose_bone.scale[0], pose_bone.scale[1], pose_bone.scale[2]))
	pose_quaternion = mathutils.Quaternion((pose_bone.rotation_quaternion[0], pose_bone.rotation_quaternion[1], pose_bone.rotation_quaternion[2], pose_bone.rotation_quaternion[3]))
	rotation = swizzle_rotation(pose_quaternion)
	rotation.normalize()
	
	return offset, scale, rotation

def parse_bone_data_path(data_path):
	if data_path is None:
		return None, None
	
	index = data_path.find("[\"")
	if index == -1:
		return None, None
	bone_name = data_path[(index + 2):]
	index = bone_name.find("\"")
	if index == -1:
		return None, None
	bone_name = bone_name[:(index)]
	
	index = data_path.rfind(".")
	if index == -1:
		return None, None
	
	return bone_name, data_path[(index + 1):]

def transform_curve(curve_points, last_frame_count, next_frame_count):
	#TODO: Might need to also divide curve_points[1] and curve_points[3] by frame counts; test to find out
	return curve_points[0] / last_frame_count + 1, curve_points[1] + 1, curve_points[2] / next_frame_count, curve_points[3]

def write_action(context, file_path, blender_action, binary_format):
	frame_rate = bpy.context.scene.render.fps / bpy.context.scene.render.fps_base
	blender_object = find_first_blender_object_with_pose() #HACK
	saved_action = blender_object.animation_data.action
	blender_object.animation_data.action = blender_action
	
	keyframe_dict = {}
	for blender_fcurve in blender_action.fcurves:
		bone_name, transform_type = parse_bone_data_path(blender_fcurve.data_path)
		if bone_name is None or transform_type is None:
			continue
		for blender_keyframe in blender_fcurve.keyframe_points:
			frame_number = blender_keyframe.co[0]
			if frame_number not in keyframe_dict:
				keyframe_dict[frame_number] = {}
			if bone_name not in keyframe_dict[frame_number]:
				keyframe_dict[frame_number][bone_name] = {}
			keyframe_dict[frame_number][bone_name][transform_type] = (blender_keyframe.handle_left[0] - blender_keyframe.co[0], blender_keyframe.handle_left[1] - blender_keyframe.co[1], blender_keyframe.handle_right[0] - blender_keyframe.co[0], blender_keyframe.handle_right[1] - blender_keyframe.co[1])
	
	keyframe_list = list(keyframe_dict.keys())
	keyframe_list.sort()
	
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"animation\",\n\t\"name\": \"")
	file.write(escape_string(blender_action.name).encode())
	#TODO: How to set loop?
	file.write(b"\",\n\t\"loop\": true,\n\t\"keyframes\": [")
	
	affected_bones = []
	IGNORE_THRESHOLD = 0.00001
	for keyframe_index in range(len(keyframe_list)):
		bpy.context.scene.frame_set(keyframe_list[keyframe_index])
		for pose_bone in blender_object.pose.bones:
			offset, scale, rotation = read_pose_bone_state(pose_bone)
			if math.fabs(offset.x) >= IGNORE_THRESHOLD or math.fabs(offset.y) >= IGNORE_THRESHOLD or math.fabs(offset.z) >= IGNORE_THRESHOLD or \
			   math.fabs(scale.x - 1.0) >= IGNORE_THRESHOLD or math.fabs(scale.y - 1.0) >= IGNORE_THRESHOLD or math.fabs(scale.z - 1.0) >= IGNORE_THRESHOLD or \
			   math.fabs(rotation.x) >= IGNORE_THRESHOLD or math.fabs(rotation.y) >= IGNORE_THRESHOLD or math.fabs(rotation.z) >= IGNORE_THRESHOLD or math.fabs(rotation.w - 1.0) >= IGNORE_THRESHOLD:
				if pose_bone.name not in affected_bones:
					affected_bones.append(pose_bone.name)
	
	first_keyframe = True
	for keyframe_index in range(len(keyframe_list)):
		if first_keyframe:
			first_keyframe = False
		else:
			file.write(b",")
		
		#glTF uses bpy.ops.nla.bake(); is it necessary?
		bpy.context.scene.frame_set(keyframe_list[keyframe_index])
		
		if keyframe_index > 0:
			last_frame_count = (keyframe_list[keyframe_index] - keyframe_list[keyframe_index - 1])
		else:
			last_frame_count = keyframe_list[-1]
		
		if keyframe_index < len(keyframe_list) - 1:
			next_frame_count = (keyframe_list[keyframe_index + 1] - keyframe_list[keyframe_index])
		else:
			next_frame_count = keyframe_list[0]
		
		keyframe_interval = next_frame_count / frame_rate
		file.write(b"\n\t\t{\n\t\t\t\"interval\": " + format_json_float(keyframe_interval).encode() + b",\n\t\t\t\"bones\": {")
		
		first_bone = True
		for pose_bone in blender_object.pose.bones:
			#TODO: This is imperfect; if a bone ever moves, it'll be encoded every frame, which is nonideal
			if pose_bone.name not in affected_bones:
				continue
			
			if first_bone:
				first_bone = False
			else:
				file.write(b",")
			file.write(b"\n\t\t\t\t\"" + pose_bone.name.encode() + b"\": {")
			offset, scale, rotation = read_pose_bone_state(pose_bone)
			offset_curve = (1, 1, 0, 0)
			scale_curve = (1, 1, 0, 0)
			rotation_curve = (1, 1, 0, 0)
			if pose_bone.name in keyframe_dict[keyframe_list[keyframe_index]]:
				if "location" in keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]:
					location_curve = transform_curve(keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]["location"], last_frame_count, next_frame_count)
				if "scale" in keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]:
					scale_curve = transform_curve(keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]["scale"], last_frame_count, next_frame_count)
				if "rotation_quaternion" in keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]:
					rotation_curve = transform_curve(keyframe_dict[keyframe_list[keyframe_index]][pose_bone.name]["rotation_quaternion"], last_frame_count, next_frame_count)
			
			file.write(b"\n\t\t\t\t\t\"offset\": {\"x\": " + format_json_float(offset.x).encode() + b", \"y\": " + format_json_float(offset.y).encode() + b", \"z\": " + format_json_float(offset.z).encode() + b"},")
			file.write(b"\n\t\t\t\t\t\"offset_curve\": {\"x_in\": " + format_json_float(offset_curve[0]).encode() + b", \"y_in\": " + format_json_float(offset_curve[1]).encode() + b", \"x_out\": " + format_json_float(offset_curve[2]).encode() + b", \"y_out\": " + format_json_float(offset_curve[3]).encode() + b"},")
			file.write(b"\n\t\t\t\t\t\"scale\": {\"x\": " + format_json_float(scale.x).encode() + b", \"y\": " + format_json_float(scale.y).encode() + b", \"z\": " + format_json_float(scale.z).encode() + b"},")
			file.write(b"\n\t\t\t\t\t\"scale_curve\": {\"x_in\": " + format_json_float(scale_curve[0]).encode() + b", \"y_in\": " + format_json_float(scale_curve[1]).encode() + b", \"x_out\": " + format_json_float(scale_curve[2]).encode() + b", \"y_out\": " + format_json_float(scale_curve[3]).encode() + b"},")
			file.write(b"\n\t\t\t\t\t\"rotation\": {\"x\": " + format_json_float(rotation.x).encode() + b", \"y\": " + format_json_float(rotation.y).encode() + b", \"z\": " + format_json_float(rotation.z).encode() + b", \"w\": " + format_json_float(rotation.w).encode() + b"},")
			file.write(b"\n\t\t\t\t\t\"rotation_curve\": {\"x_in\": " + format_json_float(rotation_curve[0]).encode() + b", \"y_in\": " + format_json_float(rotation_curve[1]).encode() + b", \"x_out\": " + format_json_float(rotation_curve[2]).encode() + b", \"y_out\": " + format_json_float(rotation_curve[3]).encode() + b"}")
			file.write(b"\n\t\t\t\t}")
		
		file.write(b"\n\t\t\t}\n\t\t}")
	
	file.write(b"\n\t],\n\t\"markers\": {")
	first_marker = True
	for pose_marker in blender_action.pose_markers:
		if first_marker:
			first_marker = False
		else:
			file.write(b",")
		file.write(b"\n\t\t\"" + escape_string(pose_marker.name).encode() + b"\": " + format_json_float(pose_marker.frame / frame_rate).encode())
	file.write(b"\n\t}\n}")
	file.close()
	
	blender_object.animation_data.action = saved_action

class ExportStem3D(bpy.types.Operator, ExportHelper):
	bl_idname = 'export_scene.stem3d'
	bl_label = 'Export Stem3D'
	
	filename_ext = '.stem3d'
	filter_glob = StringProperty(default='*.stem3d', options={'HIDDEN'})
	
	export_format = 'ASCII'
	
	def execute(self, context):
		try:
			os.mkdir(self.filepath)
		except FileExistsError:
			pass
		
		for blender_mesh in bpy.data.meshes:
			write_mesh(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_mesh.name, ".mesh")), blender_mesh, False)
		for blender_material in bpy.data.materials:
			write_material(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_material.name, ".material")), blender_material, False)
		for blender_armature in bpy.data.armatures:
			write_armature(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_armature.name, ".armature")), blender_armature, False)
		for blender_action in bpy.data.actions:
			write_action(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_action.name, ".animation")), blender_action, False)
		return {'FINISHED'}

def register():
	bpy.utils.register_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.append(menu_func_export_stem3d)

def unregister():
	bpy.utils.unregister_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.remove(menu_func_export_stem3d)

def menu_func_export_stem3d(self, context):
	self.layout.operator(ExportStem3D.bl_idname, text='stem3d (JSON)')
