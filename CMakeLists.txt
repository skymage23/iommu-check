cmake_minimum_required(VERSION 4.0.0)
# Refuse compilation on all platforms except Windows
# Re: this project uses foundational components of Windows.
if(NOT WIN32)
    message(FATAL_ERROR "This project can only be compiled on Windows operating systems.")
endif()

project(iommu-check)

add_executable(iommu-check
    ${CMAKE_CURRENT_LIST_DIR}/src/iommu-check.cc
)
target_link_libraries(iommu-check
    PRIVATE setupapi
    PRIVATE cfgmgr32
)