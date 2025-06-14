#include <allegro5/allegro_font.h>
#include <memory>
#include <sstream>
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Label.hpp"

namespace Engine {
    Label::Label(const std::string &text, const std::string &font, int fontSize, float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY) : IObject(x, y, 0, 0, anchorX, anchorY), font(Resources::GetInstance().GetFont(font, fontSize)), Text(text), Color(al_map_rgba(r, g, b, a)) {
    }
    void Label::Draw() const {
        //al_draw_text(font.get(), Color, Position.x - Anchor.x * GetTextWidth(), Position.y - Anchor.y * GetTextHeight(), 0, Text.c_str());
        std::istringstream stream(Text);
        std::string line;
        float lineHeight = al_get_font_line_height(font.get());
        int lineNum = 0;
        while (std::getline(stream, line)) {
            int lineWidth = al_get_text_width(font.get(), line.c_str());
            al_draw_text(
                font.get(),
                Color,
                Position.x - Anchor.x * lineWidth,
                Position.y - Anchor.y * GetTextHeight() + lineNum * lineHeight,
                0,
                line.c_str()
            );
            lineNum++;
        }
    }
    int Label::GetTextWidth() const {
        return al_get_text_width(font.get(), Text.c_str());
    }
    int Label::GetTextHeight() const {
        return al_get_font_line_height(font.get());
    }
}
