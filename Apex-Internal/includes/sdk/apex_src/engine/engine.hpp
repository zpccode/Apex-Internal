#pragma once
#include<math.h>
#include<cstdint>
#include<cmath>
#include<cstddef>
#include<Windows.h>

#define M_PI 3.14159265
#define RAD_TO_DEG(x) x * (180.f / M_PI)
#define DEG_TO_RAD(x) x * (M_PI / 180.f)

struct ColorRGB
{
	float r, g, b;
};

struct GlowMode
{
	BYTE NormalGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

struct GlowFlade
{
	int a, b;
	float c, d, e, f;
};

struct Matrix
{
	float matrix[16];
};

inline Matrix GetViewMatrix(uintptr_t base, uintptr_t offset, uintptr_t offset_second)
{
	uintptr_t view_renderer = *(uintptr_t*)(base + offset);
	if (view_renderer)
	{
		uintptr_t view_matrix = *(uintptr_t*)(view_renderer + offset_second);
		if (view_matrix)
		{
			Matrix view_matrix_ = *(Matrix*)(view_matrix);
			return view_matrix_;
		}
	}
}

struct Vector2 {
	float x, y;

	Vector2 operator-(Vector2 ape)
	{
		return { x - ape.x, y - ape.y };
	}

	Vector2 operator+(Vector2 ape)
	{
		return { x + ape.x, y + ape.y };
	}

	Vector2 operator*(float ape)
	{
		return { x * ape, y * ape };
	}

	Vector2 operator/(float ape)
	{
		return { x / ape, y / ape };
	}

	Vector2 operator/=(float ape)
	{
		x /= ape;
		y /= ape;

		return *this;
	}

	Vector2 operator+=(Vector2 ape)
	{
		return { x += ape.x, y += ape.y };
	}

	Vector2 operator-=(Vector2 ape)
	{
		return { x -= ape.x, y -= ape.y };
	}

	void Normalize()
	{
		if (x > 89.0f)
			x -= 180.f;

		if (x < -89.0f)
			x += 180.f;

		if (y > 180.f)
			y -= 360.f;

		if (y < -180.f)
			y += 360.f;
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float Dist2D(Vector2 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct Vector3 {
	float x, y, z;

	float distance(Vector3 vec)
	{
		return sqrt(
			pow(vec.x - x, 2) +
			pow(vec.y - y, 2)
		);
	}

	Vector3() : x(0), y(0), z(0) {}

	Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	Vector3 operator-(Vector3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	Vector3 operator+(Vector3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	Vector3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	Vector3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	Vector3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	Vector3 operator+=(Vector3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	Vector3 operator-=(Vector3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(Vector3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(Vector3 ape)
	{
		return (*this - ape).Length2D();
	}


	float Distance(Vector3 pos)
	{
		float Dist = sqrt((x - pos.x) * (x - pos.x)
			+ (y - pos.y) * (y - pos.y)
			+ (z - pos.z) * (z - pos.z));
		return Dist * 0.01905f;
	}

	Vector3 CalcAngle(Vector3 dst, float height)
	{
		Vector3 angle, e = { dst.x - x, dst.y - y, dst.z - z };
		float eh = sqrt(e.x * e.x + e.y * e.y);
		angle.x = (atan2(-e.z, eh) * 180 / 3.14159f) + height;
		angle.y = (atan2(e.y, e.x) * 180 / 3.14159f);
		angle.z = 0;
		return angle;
	}

	bool world_to_screen(Vector2& screen, uintptr_t dwbase, uintptr_t first_offset, uintptr_t second_offset) {
		float* m_vMatrix = GetViewMatrix(dwbase, first_offset, second_offset).matrix;
		float w = m_vMatrix[12] * x + m_vMatrix[13] * y + m_vMatrix[14] * z + m_vMatrix[15];

		if (w < 0.01f)
			return false;

		screen.x = m_vMatrix[0] * x + m_vMatrix[1] * y + m_vMatrix[2] * z + m_vMatrix[3];
		screen.y = m_vMatrix[4] * x + m_vMatrix[5] * y + m_vMatrix[6] * z + m_vMatrix[7];

		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;

		float x = GetSystemMetrics(SM_CXSCREEN) / 2;
		float y = GetSystemMetrics(SM_CYSCREEN) / 2;

		x += 0.5 * screen.x * GetSystemMetrics(SM_CXSCREEN) + 0.5;
		y -= 0.5 * screen.y * GetSystemMetrics(SM_CYSCREEN) + 0.5;

		screen.x = x;
		screen.y = y;

		if (screen.x > GetSystemMetrics(SM_CXSCREEN) || screen.x < 0 || screen.y > GetSystemMetrics(SM_CYSCREEN) || screen.y < 0)
			return false;

		return true;
	}
};

Vector3 RotatePoint(Vector3 EntityPos, Vector3 LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians = false)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;

	float Yaw = angle - 90.0f;
	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
	*viewCheck = y_1 < 0;
	x_1 *= zoom;
	y_1 *= zoom;
	int sizX = sizeX / 2;
	int sizY = sizeY / 2;
	x_1 += sizX;

	y_1 += sizY;
	if (x_1 < 5)
		x_1 = 5;
	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;
	if (y_1 < 5)
		y_1 = 5;
	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;
	x_1 += posX;
	y_1 += posY;
	return Vector3(x_1, y_1, 0);
}

float Distance(Vector3 EntityPos, Vector3 PlayerPos)
{
	float Dist = sqrt((PlayerPos.x - EntityPos.x) * (PlayerPos.x - EntityPos.x)
		+ (PlayerPos.y - EntityPos.y) * (PlayerPos.y - EntityPos.y)
		+ (PlayerPos.z - EntityPos.z) * (PlayerPos.z - EntityPos.z));
	return Dist * 0.01905f;
}