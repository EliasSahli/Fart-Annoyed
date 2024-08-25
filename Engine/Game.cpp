#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ball(Vec2(300.0f +24.0f, 300.0f), Vec2(-1.0f, -1.0f)),
	walls(0.0f, float(gfx.ScreenWidth), 0.0f, float(gfx.ScreenHeight)),
	soundPad(L"Sounds\\arkpad.wav"),
	soundBrick(L"Sounds\\arkbrick.wav"),
	soundGameOver(L"Sounds\\Fart1.wav"),
	pad(Vec2(100.0f, 500.0f), 50.0f, 15.0f)
{
	const Color color[4] = { Colors::Red,Colors::Green,Colors::Blue,Colors::Cyan };

	const Vec2 topleft(40.0f, 40.0f);

	int i = 0;
	for (int y = 0; y < nBricksDown; y++)
	{
		const Color c = color[y];
		for (int x = 0; x < nBricksAcross; x++)
		{
			bricks[i] = Brick(RectF(topleft + Vec2(x * brickWidth, y * brickHeight), brickWidth, brickHeight), c);
			
			i++;
		}
	}
}

void Game::Go()
{
	gfx.BeginFrame();	

	float elapsedTime = ft.Mark();
	while (elapsedTime > 0)
	{
		const float dt = std::min(0.0025f, elapsedTime);

		UpdateModel(dt);

		elapsedTime -= dt;
	}    
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel( float dt)
{
	if (!gameIsOver)
	{
		pad.Update(wnd.kbd, dt);
		pad.DoWallCollision(walls);

		ball.Update(dt);

		bool collisionHasHappened = false;
		int curColIndex;
		float curClosestDistSq;

		for (int i = 0; i < nbricks; i++)
		{
			if (bricks[i].CheckBallCollision(ball))
			{
				float newClosestDistSq = (ball.GetPos() - bricks[i].GetCenter()).GetLengthSq();
				if (collisionHasHappened)
				{
					if (newClosestDistSq < curClosestDistSq)
					{
						curClosestDistSq = newClosestDistSq;
						curColIndex = i;
					}
				}
				else
				{
					curClosestDistSq = newClosestDistSq;
					curColIndex = i;
					collisionHasHappened = true;
				}
			}
		}
		if (collisionHasHappened)
		{
			bricks[curColIndex].ExecuteBallCollision(ball);
			soundBrick.Play();
			pad.ResetCoolDown();
		}

		if (pad.DoBallCollision(ball))
		{
			soundPad.Play();
		}

		const int ballWallColResult = ball.DoWallCollision(walls);
		if (ballWallColResult == 1)
		{
			pad.ResetCoolDown();
			soundPad.Play();
		}
		else if (ballWallColResult == 2)
		{
			soundGameOver.Play();
			gameIsOver = true;
		}
	}
}

void Game::ComposeFrame()
{
	ball.Draw(gfx);

	for (const Brick& b : bricks)
	{
		b.Draw(gfx);
	}

	pad.Draw(gfx);
}
