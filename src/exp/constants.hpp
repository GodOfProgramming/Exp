#pragma once

namespace Exp
{
  namespace Cfg
  {
    namespace Dir
    {
      constexpr const char* GAME_MODELS  = "assets/json/models";
      constexpr const char* GAME_OBJECTS = "assets/json/game";
      constexpr const char* SHADERS      = "assets/json/shaders";
      constexpr const char* TEXTURES     = "assets/json/textures";
      constexpr const char* ANIMATIONS   = "assets/json/animations";
    }  // namespace Dir
    namespace File
    {
      constexpr const char* SETTINGS = "assets/settings.json";
    }
  }  // namespace Cfg
  namespace Assets
  {
    namespace Dir
    {
      constexpr const char* GAME_SCRIPTS  = "assets/scripts";
      constexpr const char* SHADER_OUTPUT = "assets/shaders/out";
      constexpr const char* TEXTURES      = "assets/textures";
      constexpr const char* UI            = "assets/ui";
    }  // namespace Dir
  }    // namespace Assets

  namespace JSON
  {
    namespace Keys
    {
      constexpr const char* SHADER_FRAGMENT          = "fragment";
      constexpr const char* SHADER_VERTEX            = "vertex";
      constexpr const char* GAME_OBJECT_ON_CONSTRUCT = "on_construct";
      constexpr const char* GAME_OBJECT_ON_UPDATE    = "on_update";
      constexpr const char* GAME_OBJECT_SHADER       = "shader";
      constexpr const char* GAME_OBJECT_MODEL        = "model";
      constexpr const char* GAME_OBJECT_ANIMATION    = "animation";
      constexpr const char* GAME_OBJECT_SCRIPT       = "script";
      constexpr const char* GAME_OBJECT_DRAW_DESC    = "draw_description";
      constexpr const char* TEXTURE_FILE             = "file";
      constexpr const char* ANIMATION_WIDTH          = "width";
      constexpr const char* ANIMATION_HEIGHT         = "height";
      constexpr const char* ANIMATION_TEXTURE        = "texture";
      constexpr const char* ANIMATION_ACTIONS        = "actions";
    }  // namespace Keys
  }    // namespace JSON

  namespace Lua
  {
    namespace Usertypes
    {
      namespace glm
      {
        constexpr const char* IVEC2 = "Geom_IVec2";
        constexpr const char* VEC2  = "Geom_Vec2";
        constexpr const char* VEC3  = "Geom_Vec3";
        constexpr const char* VEC4  = "Geom_Vec4";
      }  // namespace glm
      namespace Game
      {
        constexpr const char* INFO   = "Game_Info";
        constexpr const char* OBJECT = "Game_Object";
      }  // namespace Game
      namespace GL
      {
        constexpr const char* PROGRAM = "GL_Program";
        constexpr const char* UNIFORM = "GL_Uniform";
      }  // namespace GL
      namespace Ui
      {
        constexpr const char* FRAME     = "Ui_Frame";
        constexpr const char* TEXT_BOX  = "Ui_TextBox";
        constexpr const char* WINDOW_UI = "Ui_Window";
      }  // namespace Ui
      namespace R
      {
        constexpr const char* GAME_OBJECTS   = "R_GameObjects";
        constexpr const char* META_MODEL     = "R_ModelMeta";
        constexpr const char* META_OBJECT    = "R_ObjectMeta";
        constexpr const char* META_ANIMATION = "R_AnimationMeta";
      }  // namespace R
      namespace Render
      {
        constexpr const char* DRAW_DESCRIPTION = "Render_DrawDescription";
      }  // namespace Render
    }    // namespace Usertypes
  }      // namespace Lua

  namespace Ui
  {
    namespace Attr
    {
      constexpr const char* BUTTON    = "button";
      constexpr const char* CLICK     = "click";
      constexpr const char* COLLAPSED = "collapsed";
      constexpr const char* ON_PARSED = "on_construct";
      constexpr const char* FIXED     = "fixed";
      constexpr const char* FN        = "fn";
      constexpr const char* HEIGHT    = "height";
      constexpr const char* ID        = "id";
      constexpr const char* IF        = "if";
      constexpr const char* SCRIPT    = "script";
      constexpr const char* TITLE     = "title";
      constexpr const char* WHILE     = "while";
      constexpr const char* WIDTH     = "width";
      constexpr const char* X         = "x";
      constexpr const char* Y         = "y";
    }  // namespace Attr

    namespace El
    {
      constexpr const char* BUTTON   = "button";
      constexpr const char* FRAME    = "frame";
      constexpr const char* REPEAT   = "repeat";
      constexpr const char* SAMELINE = "sameline";
      constexpr const char* TEXT_BOX = "text";
      constexpr const char* WINDOW   = "window";
    }  // namespace El
  }    // namespace Ui

}  // namespace Exp
