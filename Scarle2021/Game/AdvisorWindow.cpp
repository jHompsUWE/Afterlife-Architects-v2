#include "pch.h"
#include "AdvisorWindow.h"

#include "DataManager.h"


AdvisorWindow::AdvisorWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
                             std::string _filepath, Vector2 _setScale) : UIWindow()
{
    // Setup for window background
    windowBackGround = new ImageGO2D(_filepath, _d3dDevice);
    windowBackGround->SetOrigin(Vector2(0, 0));
    windowBackGround->SetScale(Vector2(0.5,0.5));

    // Sets window res
    window_res = Vector2(windowBackGround->GetRes().x
        * windowBackGround->GetScale().x, windowBackGround->GetRes().y
        * windowBackGround->GetScale().y);

    // Sets window pos
    window_pos = _windowPosition;
    windowBackGround->SetPos(window_pos);
    
    for (int i = 0; i < 5; i++)
    {
        buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 66, window_pos.y + 158 + 15 * i),
            DataManager::GetD3DDevice(), "ButtonAdvisor",
            AL::EventType::event_ui, AL::UI::Action(AL::UI::adv_option1 + i), 0, Vector2(0.225, 0.25)));
    }
    
    // Texts
    for (int i = 0; i < 5; i++)
    {
        indicators_ar.push_back(new ImageGO2D("adv_heaven", DataManager::GetD3DDevice()));
        indicators_ar[i]->SetPos(Vector2(window_pos.x + 26, window_pos.y + 158 + (i*15)));
        indicators_ar[i]->SetScale(Vector2(1,1));
        indicators_ja.push_back(new ImageGO2D("adv_hell", DataManager::GetD3DDevice()));
        indicators_ja[i]->SetPos(Vector2(window_pos.x + 106, window_pos.y + 158 + (i*15)));
        indicators_ja[i]->SetScale(Vector2(1,1));
    
        text_vec.push_back(new TextGO2D("Box"));
        text_vec[i]->SetPos(Vector2(window_pos.x + 46, window_pos.y + 148 + (i*15)));
        text_vec[i]->SetScale(Vector2(0.3, 0.3));
    }

    // Image vector for Jasper and Aria
    for (int i = 0; i < 13; i++)
    {
        image_vec_ar.push_back(new ImageGO2D(advisor_filenames_ar[i], DataManager::GetD3DDevice()));
        image_vec_ar[i]->SetPos(Vector2(window_pos.x + 260, window_pos.y + 80));
        image_vec_ar[i]->SetScale(Vector2(1, 1));
    }
    for (int j = 0; j < 12; j++)
    {
        image_vec_ja.push_back(new ImageGO2D(advisor_filenames_ja[j], DataManager::GetD3DDevice()));
        image_vec_ja[j]->SetPos(Vector2(window_pos.x + 100, window_pos.y + 80));
        image_vec_ja[j]->SetScale(Vector2(1, 1));
    }
    
    // Text vector
    text_vec.push_back(new TextGO2D(""));
    text_vec[5]->SetPos(Vector2(window_pos.x+120,window_pos.y+150));
    text_vec[5]->SetScale(Vector2(0.3,0.3));
}

AdvisorWindow::~AdvisorWindow()
{
    //Deletes images for Aria And Jasper 
    for (auto image : image_vec_ar)
    {
        delete image;        
    }
    for (auto image : image_vec_ja)
    {
        delete image;        
    }
    for (auto image : indicators_ar)
    {
        delete image;        
    }
    for (auto image : indicators_ja)
    {
        delete image;        
    }
}

void AdvisorWindow::update(GameData* _gameData, Vector2& _mousePosition)
{
    // Stop if not visible
    if (!is_visible) return;

    mouse_pos = _mousePosition; 
    
    // Updates buttons
    for (const auto& button : buttons)
    {
        button->update(_gameData);
    }
    // Updates image + text
    for (auto image_ar : image_vec_ar)
    {
        image_ar->Tick(_gameData);      
    }
    for (auto image_ja : image_vec_ja)
    {
        image_ja->Tick(_gameData);      
    }
    for (auto ind_ar : indicators_ar)
    {
        ind_ar->Tick(_gameData);      
    }
    for (auto ind_ja : indicators_ja)
    {
        ind_ja->Tick(_gameData);      
    }
    for (auto& text : text_vec)
    {
        text->Tick(_gameData);
    }

    windowBackGround->Tick(_gameData);

    inside = isInside(mouse_pos);
    
    //if clicked updates pos and scale for window drag  
    if (toggle_click && inside)
    {

        //new pos on click and drag 
        const Vector2 offset = old_mouse_pos - mouse_pos;
        windowBackGround->SetPos(windowBackGround->GetPos()-offset);
        window_pos = windowBackGround->GetPos();
        
        //button pos
        for (auto& button : buttons)
        {
            Vector2 const button_pos = button->getPosition();
            button->setPostion(button_pos - offset);
        }

        //image pos
        for (auto& image : image_vec_ar)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        for (auto& image : image_vec_ja)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        for (auto& image : indicators_ar)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        for (auto& image : indicators_ja)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        //text pos
        for (auto& text : text_vec)
        {
            Vector2 const button_pos = text->GetPos();
            text->SetPos(button_pos - offset);
        }
    }
    old_mouse_pos = mouse_pos;
}

void AdvisorWindow::render(DrawData2D* _drawData)
{
    UIWindow::render(_drawData);
    
    if(!is_visible) return;
    
    // Render Aria, Jasper + indicators
    for (int i = 0; i < 13; i++)
    {
        if (i == pointed_image_ar)
        {
            image_vec_ar[i]->Draw(_drawData);
        }
    }
    for (int j = 0; j < 12; j++)
    {
        if (j == pointed_image_ja)
        {
            image_vec_ja[j]->Draw(_drawData);
        }
    }
    for (int k = 0; k < 5; k++)
    {
        if (show_ind_ar[k])
        {
            indicators_ar[k]->Draw(_drawData);
        }
        if (show_ind_ja[k])
        {
            indicators_ja[k]->Draw(_drawData);
        }
    }
}

void AdvisorWindow::reSize(Vector2 game_res)
{   
    auto& scale = windowBackGround->ReSize(game_res.x, game_res.y);
    //reScales background
    window_res *= scale;
    window_pos *= scale;
    
    // resize buttons of UI window
    for (const auto& button : buttons)
    {
        button->reSize(game_res);
    }
    for (auto text : text_vec)
    {
        text->ReSize(game_res.x, game_res.y);
    }
    for (auto image : image_vec_ar)
    {
        image->ReSize(game_res.x, game_res.y);
    }
    for (auto image : image_vec_ja)
    {
        image->ReSize(game_res.x, game_res.y);
    }
    for (auto image : indicators_ar)
    {
        image->ReSize(game_res.x, game_res.y);
    }
    for (auto image : indicators_ja)
    {
        image->ReSize(game_res.x, game_res.y);
    }
    for (auto text : text_vec)
    {
        text->ReSize(game_res.x, game_res.y);
    }
}

/// <summary>
/// Show aria image from the image pool
/// </summary>
/// <param name="filename">String filename for image to show</param>
void AdvisorWindow::setAriaImage(string filename)
{
    for (int i = 0; i < 13; i++)
    {
        if (advisor_filenames_ar[i] == filename)
        {
            pointed_image_ar = i;
        }
    }
}

/// <summary>
/// Show jasper image from the image pool
/// </summary>
/// <param name="filename">String filename for image to show</param>
void AdvisorWindow::setJasperImage(string filename)
{
    for (int i = 0; i < 12 ; i ++)
    {
        if (advisor_filenames_ja[i] == filename)
        {
            pointed_image_ja = i;
        }
    }
}

void AdvisorWindow::setText(string new_string)
{
    text_vec[5]->ChangeString(new_string);
}

/// <summary>
/// Update side options text + indicator visal
/// </summary>
/// <param name="box_num"> Int index for which textbox to update</param>
/// <param name="indicator"> Which indicators to show based off int acting as Enum </param>
/// <param name="title"> String for new text for option box </param>
void AdvisorWindow::setOptionBox(int box_num, int indicator, string title)
{
    text_vec[box_num]->ChangeString(title);
    switch (indicator)
    {
    case 0:
        show_ind_ar[box_num] = true;
        show_ind_ja[box_num] = false;
        break;
    case 1:
        show_ind_ar[box_num] = false;
        show_ind_ja[box_num] = true;
        break;
    case 2:
        show_ind_ar[box_num] = true;
        show_ind_ja[box_num] = true;
        break;
    case 3:
        show_ind_ar[box_num] = false;
        show_ind_ja[box_num] = false;
        break;
    }
}