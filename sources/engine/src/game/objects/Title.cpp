#include "Title.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"

namespace game {

//----------------------------------------------------------------------------//
renderer::Sprite2D* getSpriteForDescription(char description)
{
    switch (description)
    {
    case '0':
        return ResourcesManager::getSprite2D("brickWall_All");
    case '1':
        return ResourcesManager::getSprite2D("brickWall_TopLeft");
    case '2':
        return ResourcesManager::getSprite2D("brickWall_TopRight");
    case '3':
        return ResourcesManager::getSprite2D("brickWall_Top");
    case '4':
        return ResourcesManager::getSprite2D("brickWall_BottomLeft");
    case '5':
        return ResourcesManager::getSprite2D("brickWall_Left");
    case '6':
        return ResourcesManager::getSprite2D("brickWall_TopRight_BottomLeft");
    case '7':
        return ResourcesManager::getSprite2D("brickWall_Top_BottomLeft");
    case '8':
        return ResourcesManager::getSprite2D("brickWall_BottomRight");
    case '9':
        return ResourcesManager::getSprite2D("brickWall_TopLeft_BottomRight");
    case 'A':
        return ResourcesManager::getSprite2D("brickWall_Right");
    case 'B':
        return ResourcesManager::getSprite2D("brickWall_Top_BottomRight");
    case 'C':
        return ResourcesManager::getSprite2D("brickWall_Bottom");
    case 'D':
        return ResourcesManager::getSprite2D("brickWall_TopLeft_Bottom");
    case 'E':
        return ResourcesManager::getSprite2D("brickWall_TopRight_Bottom");
    case '.':
        return ResourcesManager::getSprite2D("empty");
    default:
        LOG_WARN("Unknown Start Level description: '{}'", description);
        return ResourcesManager::getSprite2D("empty");
    }
}

//----------------------------------------------------------------------------//
Title::Title(EGameState game_state)
    : StaticObject(tps::Vec2i{}, tps::Vec2i{})
{
    std::vector<std::string> titleDescription{};
    switch (game_state)
    {
    case EGameState::StartScreen:
        titleDescription = ResourcesManager::getTitleDescription("start_screen");
        break;
    case EGameState::Pause:
        titleDescription = ResourcesManager::getTitleDescription("pause");
        break;
    case EGameState::GameOver:
        titleDescription = ResourcesManager::getTitleDescription("game_over");
        break;
    default:
        break;
    }
    if (titleDescription.empty())
    {
        LOG_WARN("Empty game title description\n");
        return;
    }
    this->setSize(
        static_cast<int>(titleDescription.front().length() * BLOCK_SIZE),
        static_cast<int>(titleDescription.size() * BLOCK_SIZE)
    );

    int topOffset{};
    for (auto currentRow : titleDescription)
    {
        int leftOffset{};
        for (auto currentElement : currentRow)
        {
            this->m_Sprites.emplace_back(
                getSpriteForDescription(currentElement), tps::Vec2i(leftOffset, topOffset)
            );
            this->m_Sprites.back().first->setSize(BLOCK_SIZE, BLOCK_SIZE);

            leftOffset += BLOCK_SIZE;
        }

        topOffset += BLOCK_SIZE;
    }
}

//----------------------------------------------------------------------------//
void Title::render() const
{
    for (const auto currentElement : this->m_Sprites)
    {
        currentElement.first->render(this->getPosition() + currentElement.second);
    }
}


} /* !namespace game */
