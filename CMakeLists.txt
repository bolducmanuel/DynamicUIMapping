cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include(ScoreExternalAddon)
endif()

if(NOT TARGET score_plugin_avnd)
  return()
endif()

project(score_addon_hybridation LANGUAGES CXX)

avnd_score_plugin_init(
  BASE_TARGET score_addon_hybridation
)

avnd_score_plugin_add(
  BASE_TARGET score_addon_hybridation
  SOURCES
    hybridation/hybridation.hpp
    hybridation/hybridationModel.hpp
    hybridation/hybridationModel.cpp
    hybridation/hybridationUi.hpp
  TARGET hybridation
  MAIN_CLASS hybridation
  NAMESPACE Example
)

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_hybridation
  PLUGIN_VERSION 1
  PLUGIN_UUID "ea89b5d4-70d6-4148-afe6-b6034b27f6b4"
)
