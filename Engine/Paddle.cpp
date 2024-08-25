#include "Paddle.h"

Paddle::Paddle(const Vec2& pos_in, float halfWidth_in, float halfHeight_in)
	:
	pos(pos_in),
	halfWidth(halfWidth_in),
	halfHeight(halfHeight_in)
{
}

void Paddle::Draw(Graphics& gfx) const
{
	RectF rect = GetRect();
	gfx.DrawRect(rect,wingColor);
	rect.left += wingWidth;
	rect.right -= wingWidth;
	gfx.DrawRect(rect, color);
}

bool Paddle::DoBallCollision(Ball& ball) 
{
	if (!hasCoolDown)
	{
		const RectF rect = GetRect();
		if (rect.isOverLappingWith(ball.GetRect()))
		{
			const Vec2 ballPos = ball.GetPos();
			if (std::signbit(ball.GetVelocity().x) == std::signbit((ballPos - pos).x) || ballPos.x >= rect.left && ballPos.x <= rect.right)
			{
				Vec2 dir;
				const float xDifference = ballPos.x - pos.x;
				const float fixedXComponent = fixedZoneHalfWidth * exitFactor;
				if (std::abs(xDifference) < fixedZoneHalfWidth)
				{
					if (xDifference < 0.0f)
					{
						dir = Vec2(-fixedXComponent, -1.0f);
					}
					else
					{
						dir = Vec2(fixedXComponent, -1.0f);
					}
				}
				else
				{
					dir = Vec2 (xDifference * exitFactor, -1.0f);
				}

				ball.SetDirection(dir);
			}
			else
			{
				ball.ReboundX();
			}
			hasCoolDown = true;
			return true;
		}
	}
	return false;
}

void Paddle::DoWallCollision(const RectF& walls)
{
	const RectF rect = GetRect();
	if (rect.left < walls.left)
	{
		pos.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right)
	{
		pos.x -= rect.right - walls.right;
	}
}

void Paddle::Update( const Keyboard & kbd,float dt )
{
	if( kbd.KeyIsPressed( VK_LEFT ) )
	{
		pos.x -= speed * dt;
	}
	if( kbd.KeyIsPressed( VK_RIGHT ) )
	{
		pos.x += speed * dt;
	}
}

RectF Paddle::GetRect() const
{
	return RectF::FromCenter(pos,halfWidth,halfHeight);
}

void Paddle::ResetCoolDown()
{
	hasCoolDown = false;
}
