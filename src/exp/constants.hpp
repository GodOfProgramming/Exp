#pragma once

#define LUA_FIELD(obj, field) #field, &obj::field

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
      constexpr const char* SHADER_FRAGMENT = "fragment";
      constexpr const char* SHADER_UNIFORM  = "uniforms";
      constexpr const char* SHADER_VERTEX   = "vertex";
    }  // namespace Keys
  }    // namespace JSON

  namespace Lua
  {
    namespace Usertypes
    {
      constexpr const char* DRAW_DESCRIPTION = "DrawDescription";
      constexpr const char* FRAME            = "Frame";
      constexpr const char* GAME_OBJECTS     = "GameObjects";
      constexpr const char* GLM_IVEC2        = "ivec2";
      constexpr const char* GLM_VEC2         = "vec2";
      constexpr const char* GLM_VEC3         = "vec3";
      constexpr const char* GLM_VEC4         = "vec4";
      constexpr const char* INFO             = "Info";
      constexpr const char* META_MODEL       = "ModelMeta";
      constexpr const char* META_OBJECT      = "ObjectMeta";
      constexpr const char* OBJECT           = "Object";
      constexpr const char* PROGRAM          = "ShaderProgram";
      constexpr const char* TEXT_BOX         = "TextBox";
      constexpr const char* UNIFORM          = "Uniform";
      constexpr const char* WINDOW_UI        = "Window";
    }  // namespace Usertypes

    namespace Globals
    {
      constexpr const char* WINDOW    = "window";
      constexpr const char* GAME_INFO = "game_info";
    }  // namespace Globals
  }    // namespace Lua

  namespace Ui
  {
    namespace Attr
    {
      constexpr const char* BUTTON    = "button";
      constexpr const char* CLICK     = "click";
      constexpr const char* COLLAPSED = "collapsed";
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
