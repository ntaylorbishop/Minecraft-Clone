#include "Engine/Renderer/RGBA.hpp"

#define STATIC

STATIC const RGBA RGBA::WHITE(1.f, 1.f, 1.f, 1.f);
STATIC const RGBA RGBA::BLACK(0.f, 0.f, 0.f, 1.f);
STATIC const RGBA RGBA::RED(1.f, 0.f, 0.f, 1.f);
STATIC const RGBA RGBA::GREEN(0.f, 1.f, 0.f, 1.f);
STATIC const RGBA RGBA::BLUE(0.f, 0.f, 1.f, 1.f);
STATIC const RGBA RGBA::YELLOW(1.f, 1.f, 0.f, 1.f);
STATIC const RGBA RGBA::MAGENTA(1.f, 0.f, 1.f, 1.f);
STATIC const RGBA RGBA::CYAN(0.f, 1.f, 1.f, 1.f);

RGBA::RGBA() :
    m_r(0.f),
    m_g(0.f),
    m_b(0.f),
    m_a(0.f)
{}
RGBA::RGBA(float r, float g, float b, float a) :
m_r(r),
m_g(g),
m_b(b),
m_a(a)
{}
RGBA::RGBA(int r, int g, int b, int a) {
	ConvertHex(r, g, b, a);
}

void RGBA::ConvertHex(int r, int g, int b, int a) {
	m_r = ((float)r / 255.f);
	m_g = ((float)g / 255.f);
	m_b = ((float)b / 255.f);
	m_a = ((float)a / 255.f);
}

float RGBA::r() const {
	return m_r;
}
float RGBA::g() const {
	return m_g;
}
float RGBA::b() const {
	return m_b;
}
float RGBA::a() const {
	return m_a;
}