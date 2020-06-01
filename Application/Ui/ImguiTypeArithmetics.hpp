#pragma once
#include "ThirdParty/imgui/imgui.h"


ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs);

ImVec2 &operator+=(ImVec2 &lhs, const ImVec2 &rhs);


ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs);

ImVec2 &operator-=(ImVec2 &lhs, const ImVec2 &rhs);


ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs);

ImVec2 &operator*=(ImVec2 &lhs, const ImVec2 &rhs);


ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs);

ImVec2 &operator/=(ImVec2 &lhs, const ImVec2 &rhs);


ImVec2 operator+(const ImVec2 &lhs, float scalar);

ImVec2 &operator+=(ImVec2 &lhs, float scalar);


ImVec2 operator-(const ImVec2 &lhs, float scalar);

ImVec2 &operator-=(ImVec2 &lhs, float scalar);


ImVec2 operator*(const ImVec2 &lhs, float scalar);

ImVec2 &operator*=(ImVec2 &lhs, float scalar);


ImVec2 operator/(const ImVec2 &lhs, float scalar);

ImVec2 &operator/=(ImVec2 &lhs, float scalar);




