#include "ThreatObject.h"
#include <vector>

class FlyingEnemy : public ThreatObject {
public:
    FlyingEnemy();
    ~FlyingEnemy();

    void HandleMove(const int& x_border, const int& y_border);
    void Show(SDL_Renderer* des);

    class Bomb {
    public:
        Bomb();
        ~Bomb();

        void HandleMove(const int& x_border, const int& y_border);
        void Render(SDL_Renderer* des);
        void Free();

        int x;
        int y;
        bool is_move;
        SDL_Surface* bomb_surface;
        SDL_Texture* bomb_texture;
    };

    void DropBomb(SDL_Renderer* des);

private:
    std::vector<Bomb*> bomb_list_;
};
