#include<Windows.h>

namespace aimbot
{
	bool enable_aimbot = false;
	bool draw_aim_fov = false;
	
	static float aim_fov = 1.f;
	bool smooth_aim = false;
	static float smooth_value = 1.f;

	bool slient_aim = false;
	bool npcs = false;
	bool no_recoil = false;

	float aim_dist = 5000;

	namespace hitboxes
	{
		static int player_hitbox_items = 0;
		const char* player_hitbox_chars[5] = { "Head", "Neck", "Body", "Legs", "Feet" };

		static int npcs_hitbox_items = 0;
		const char* npcs_hitbox_chars[5] = { "Head", "Neck", "Body", "Legs", "Feet" };
	}

	namespace hitbox_set
	{
		static bool head_enabled = false;
		static bool neck_enabled = false;
		static bool body_enabled = false;
		static bool legs_enabled = false;
		static bool feet_enabled = false;
	}

	namespace aim_type
	{
		static int aim_items = 0;
		const char* aim_chars[3] = { "Closest", "Fov", "Cursor" };
	}

	static bool fov_closest = false;
	static bool fov_aim = false;
	static bool cursor = false;
}

namespace triggerbot
{
	
}

namespace visuals
{
	bool enable_visuals = false;
	bool enable_outline = false;
	bool enable_chams = false;

	bool item_esp = false;
	
	bool enable_box = false;
	bool name_esp = false;
	bool health_esp = false;

	bool weapon_chams = false;

	bool enable_thirdperson = false;

	static float view_fov = 90.f;

	namespace colors {
		float glow_color[3];
		float chams_color[3];
	}

	bool init_skin_changer = false;
	bool skin_roller = false;
	int weapon_skin_id = 0;
	
	bool render_radar_option = false;
	bool render_frame_window = false;
}

namespace misc
{
	bool unlock_all = false;
	bool render_frame_window = false;
}

