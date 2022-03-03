add_executable(mono_euroc_evo
        Examples/Monocular/mono_euroc_evo.cc)
target_link_libraries(mono_euroc_evo ${PROJECT_NAME})


add_executable(mono_tum_evo
Examples/Monocular/mono_tum_evo.cc)
target_link_libraries(mono_tum_evo ${PROJECT_NAME})


add_executable(mono_ICL-NUIM_evo
Examples/Monocular/mono_ICL-NUIM_evo.cc)
target_link_libraries(mono_ICL-NUIM_evo ${PROJECT_NAME})






