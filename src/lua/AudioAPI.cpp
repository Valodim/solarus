/*
 * Copyright (C) 2006-2012 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "lua/Script.h"
#include "lowlevel/Sound.h"
#include "lowlevel/Music.h"
#include <lua.hpp>

const std::string Script::audio_module_name = "sol.audio";

/**
 * @brief Initializes the audio features provided to Lua.
 */
void Script::register_audio_module() {

  static const luaL_Reg functions[] = {
      { "play_sound", audio_api_play_sound },
      { "preload_sounds", audio_api_preload_sounds },
      { "play_music", audio_api_play_music },
      { "stop_music", audio_api_stop_music },
      { NULL, NULL }
  };
  register_functions(audio_module_name, functions);
}

/**
 * @brief Implementation of \ref lua_api_audio_play_sound.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::audio_api_play_sound(lua_State *l) {

  const SoundId& sound_id = luaL_checkstring(l, 1);
  Sound::play(sound_id);
  return 0;
}

/**
 * @brief Implementation of \ref lua_api_audio_preload_sounds.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::audio_api_preload_sounds(lua_State *l) {

  Sound::load_all();
  return 0;
}


/**
 * @brief Implementation of \ref lua_api_audio_play_music.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::audio_api_play_music(lua_State *l) {

  Script& script = get_script(l);
  const MusicId& music_id = luaL_checkstring(l, 1);

  Music::play(music_id);
  script.music_played = true;

  return 0;
}

/**
 * @brief Implementation of \ref lua_api_audio_stop_music.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::audio_api_stop_music(lua_State* l) {

  Script& script = get_script(l);

  Music::play(Music::none);
  script.music_played = true;

  return 0;
}

