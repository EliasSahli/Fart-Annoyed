#include "Brick.h"
#include "assert.h"
#include "cmath"

Brick::Brick(const RectF& rect_in, Color color_in)
	:
	rect(rect_in),
	color(color_in)
{
}

void Brick::Draw(Graphics& gfx) const
{
	if (!destroyed)
	{
		gfx.DrawRect(rect.GetExpanded(-padding), color);
	}
}

bool Brick::CheckBallCollision(const Ball& ball) const
{
	return (!destroyed && rect.isOverLappingWith(ball.GetRect()));
}

void Brick::ExecuteBallCollision(Ball& ball)
{
	assert(CheckBallCollision(ball));

	const Vec2 rectPos = GetCenter();
	const Vec2 ballPos = ball.GetPos();
	if (std::signbit(ballPos.x - rectPos.x) == std::signbit(ball.GetVelocity().x))
	{
		ball.ReboundY();
	}
	else if (ballPos.x >= rect.left && ballPos.x <= rect.right)
	{
		ball.ReboundY();
	}
	else
	{
		ball.ReboundX();
	}
	destroyed = true;
}

Vec2 Brick::GetCenter() const
{
	return rect.GetCenter();
}
