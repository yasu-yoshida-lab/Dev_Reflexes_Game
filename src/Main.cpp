# include <Siv3D.hpp> // OpenSiv3D v0.6.2

#if 0
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
	Effect effect;
	Circle circle{ Scene::Center(), 30 };
	double baseHue = 180.0;

	while (System::Update())
	{
		if (circle.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (circle.leftClicked())
		{
			effect.add<StarEffect>(Cursor::PosF(), baseHue);
			circle.center = RandomVec2(Scene::Rect().stretched(-80));
			baseHue = Random(0.0, 360.0);
		}

		circle.draw(HSV{ baseHue });
		effect.update();
	}
}
#else
# include <Siv3D.hpp>

void Main()
{
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	// テクスチャー
	const Texture texture{ U"example/planet05.png" };

	// フォント
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// テクスチャークリック範囲
	const Circle texture_cricle{ Scene::Center().x, Scene::Center().y, 100};

	//表示サイズ（倍率）
	double texture_size = 0.1;

	// クッキーの個数
	double cookies = 0;

	while (System::Update())
	{
		if (KeyF11.up() == true) {
			Window::SetFullscreen( ( Window::GetState().fullscreen == false ? true : false) );
		}

		// 円上にマウスカーソルがあれば
		if (texture_cricle.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		// 左クリックされたら
		if (texture_cricle.leftClicked())
		{
			texture_size = 0.1;
			++cookies;
		}

		// 表示サイズを回復する
		texture_size += Scene::DeltaTime();

		if (0.3 < texture_size)
		{
			texture_size = 0.3;
		}

		// 背景を描く
		Rect{ 0, 0, 800, 600 }.draw(Arg::top = ColorF{ 0.6, 0.5, 0.3 }, Arg::bottom = ColorF{ 0.2, 0.5, 0.3 });

		// クリック数を整数で表示する
		font(U"{:.0f}"_fmt(cookies)).drawAt(60, Scene::Center().x, Scene::Center().y / 4);

		// 描画する
		texture.scaled(texture_size).drawAt(texture_cricle.center);
	}
}
#endif
