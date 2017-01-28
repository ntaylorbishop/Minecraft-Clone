#pragma once

class RGBA {
public:
	RGBA();

	//Normalized RGBA values
	RGBA(float r, float g, float b, float a);

	//RGBA values in hex
	RGBA(int r, int g, int b, int a);

	//Normalizes this color's RGBA's to the given hex values
	void ConvertHex(int r, int g, int b, int a);

	float r() const;
	float g() const;
	float b() const;
	float a() const;

public:
	static const RGBA WHITE;
	static const RGBA BLACK;
	static const RGBA RED;
	static const RGBA GREEN;
	static const RGBA BLUE;
	static const RGBA YELLOW;
	static const RGBA MAGENTA;
	static const RGBA CYAN;

private:
	float m_r;
	float m_g;
	float m_b;
	float m_a;
};