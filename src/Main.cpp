# include <Siv3D.hpp> // OpenSiv3D v0.6.2

struct StarEffect : IEffect
{
	static constexpr Vec2 Gravity{ 0, 160 };

	struct Star
	{
		Vec2 start;
		Vec2 velocity;
		ColorF color;
	};

	Array<Star> m_stars;

	StarEffect(const Vec2& pos, double baseHue)
	{
		for (int32 i = 0; i < 6; ++i)
		{
			const Vec2 velocity = RandomVec2(Circle{ 60 });
			Star star{
				.start = (pos + velocity),
				.velocity = velocity,
				.color = HSV{ baseHue + Random(-20.0, 20.0) },
			};
			m_stars << star;
		}
	}

	bool update(double t) override
	{
		t /= 0.4;

		for (auto& star : m_stars)
		{
			const Vec2 pos = star.start
				+ star.velocity * t + 0.5 * t * t * Gravity;

			const double angle = (pos.x * 3_deg);

			Shape2D::Star((30 * (1.0 - t)), pos, angle)
				.draw(star.color);
		}

		return (t < 1.0);
	}
};

void Main()
{
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);

	// テクスチャー
	const Texture texture{ U"👾"_emoji };

	// フォント
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// テクスチャークリック範囲
	Circle texture_cricle{ Scene::Center().x, Scene::Center().y, 80};

	/* エフェクト */
	Effect effect;

	double baseHue = 180.0;

	while (System::Update()) {
		if (KeyF11.up() == true) {
			Window::SetFullscreen( ( Window::GetState().fullscreen == false ? true : false) );
		}

		if (texture_cricle.mouseOver()) {
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (texture_cricle.leftClicked()) {
			effect.add<StarEffect>(Cursor::PosF(), baseHue);
			texture_cricle.center = RandomVec2(Scene::Rect().stretched(-80));
			baseHue = Random(0.0, 360.0);
		}
	
		// 描画する
		texture.scaled(1.0).drawAt(texture_cricle.center);
		effect.update();
	}
}
