#include "LevelStart.hpp"
#include "renderer/Sprite2D.hpp"
#include "renderer/Text2D.hpp"
#include "game/states/Level.hpp"
#include "game/ResourcesManager.hpp"

#include <string>
#include <sstream>
#include <iomanip>


//----------------------------------------------------------------------------//
game::LevelStart::LevelStart(Level* parent_level)
	: LevelState(parent_level, tps::Vec2i{ WINDOW_WIDTH, WINDOW_HEIGHT / 2 })
	, m_pStageText  { std::make_unique<renderer::Text2D>("pixel_font") }
{
	this->initText();
	this->m_pBorder->setSize(this->m_BorderSize);
	this->m_StateTimer.setCallback(
		[this] { this->m_eStateType = ELevelStateType::ShowBorder; }
	);
	this->m_StateTimer.start(2);
}

//----------------------------------------------------------------------------//
void game::LevelStart::render() const
{
	this->m_pBorder->render(tps::Vec2i{}, this->m_BorderSize);
	this->m_pBorder->render(
		tps::Vec2i{ 0, WINDOW_HEIGHT - this->m_BorderSize.y },
		this->m_BorderSize
	);

	if (this->m_eStateType == ELevelStateType::ShowInfo)
	{
		this->m_pStageText->render();
	}
}

//----------------------------------------------------------------------------//
void game::LevelStart::update()
{
	if (this->m_eStateType == ELevelStateType::ShowInfo)
	{
		this->m_StateTimer.update();
		return;
	}

	this->m_BorderSize.y -= BLOCK_SIZE;
	if (this->m_BorderSize.y <= BLOCK_SIZE)
	{
		this->m_pParentLevel->run();
	}

}

//----------------------------------------------------------------------------//
void game::LevelStart::initText()
{
	std::ostringstream osstrm{};

	osstrm << "STAGE " << std::setw(2) << this->m_pParentLevel->getIndex();
	this->m_pStageText->setString(osstrm.str());
	this->m_pStageText->setFillColor(255, 255, 255);
	this->m_pStageText->setPosition(
		(WINDOW_WIDTH - this->m_pStageText->getSize().x) / 2,
		(WINDOW_HEIGHT - this->m_pStageText->getSize().y) / 2
	);
}
