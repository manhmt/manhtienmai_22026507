
#include "Menu.h"
Menu::Menu(SDL_Renderer* renderer) :
    selectedOption(0),
    optionText(),
    menuUpdated(true),
    backgroundTexture(),
    instructionText(),
    renderer(renderer)
    //isInLevelSelection(false)


{
    //nderer = g_screen;  // Add this line
    // ...

    // Initialize the font
    font = TTF_OpenFont("font//dlxfont_.ttf", 48); // Replace with your font file path and desired size
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // Load background image and create a texture
    SDL_Surface* backgroundImage = IMG_Load("img//menu.jpg");
    if (backgroundImage == nullptr) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
        SDL_FreeSurface(backgroundImage);
    }

    instructionText.SetColor(TextObject::WHITE_TEXT);
    instructionText.SetText("Please push Enter button to play game"); // Add your desired instruction text
    instructionText.LoadFromRenderText(font, renderer);
}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
}

void Menu::addOption(const std::string& option) {
    options.push_back(option);
}
//void Menu::renderLevelSelection() {
    //options.clear();
    //addOption("Easy");
    //addOption("Medium");
   // addOption("Hard");
   // isInLevelSelection = true;
   // renderMenu();
//}
void Menu::renderMenu() {
    if (menuUpdated) {
        std::cout << "Rendering menu option "<< std::endl;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        // The rest of the renderMenu() function stays the same
        for (int i = 0; i < options.size(); ++i) {
            // Set the text color based on whether the option is selected or not
            std::cout << "Rendering menu option " << i << std::endl;
            if (i == selectedOption) {
                optionText.SetColor(TextObject::WHITE_TEXT);
            }
            else {
                optionText.SetColor(TextObject::BLACK_TEXT);
            }

            // Render the text on the screen at the appropriate position
            optionText.SetText(options[i]);
            optionText.LoadFromRenderText(font, renderer);
            optionText.RenderText(renderer, 250, 160 + i * 60); // Adjust x and y positions as needed
        }

        SDL_RenderPresent(renderer);

        menuUpdated = false;
    }
}

// The rest of the functions stay the same


void Menu::moveUp() {
    if (selectedOption > 0) {
        selectedOption--;
    }
}

void Menu::moveDown() {
    if (selectedOption < options.size() - 1) {
        selectedOption++;
    }
}

    int Menu::processMouseClick(int x, int y, TTF_Font* font, SDL_Renderer* renderer)
    {
        int textPosX = 350; // The fixed x position of the text
        int verticalSpacing = 60; // The vertical spacing between menu options

        for (int i = 0; i < options.size(); ++i)
        {
            int textWidth, textHeight;
            TTF_SizeText(font, options[i].c_str(), &textWidth, &textHeight);

            int textPosY = 160 + i * verticalSpacing; // Calculate the y position based on the index

            if (x >= textPosX && x <= textPosX + textWidth &&
                y >= textPosY && y <= textPosY + textHeight)
            {
                selectedOption = i;

                menuUpdated = true;
                if (selectedOption == INSTRUCTION) {
                    showInstructions();
                }

                return i;
            }
        }
        //isInLevelSelection = false;
        return -1;
    }
void Menu::showInstructions() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load and render the instruction image
    SDL_Surface* instructionImage = IMG_Load("img//instruction.jpg");
    if (instructionImage == nullptr) {
        std::cerr << "Failed to load instruction image: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionImage);
    SDL_FreeSurface(instructionImage);

    SDL_RenderCopy(renderer, instructionTexture, nullptr, nullptr);
    instructionText.RenderText(renderer, 250, 200); // Adjust the position as needed
    SDL_RenderPresent(renderer);

    // Wait for the user to press a key to return to the menu
    SDL_Event event;
    bool instructionsShown = true;

    while (instructionsShown) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                instructionsShown = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    instructionsShown = false;
                    break;
                }
            }
        }
    }

    // Clean up the instruction texture
    SDL_DestroyTexture(instructionTexture);
}

