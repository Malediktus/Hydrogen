#pragma once

#include <Hydrogen/Hydrogen.hpp>

namespace HydrogenEditor {
class Panel : public Hydrogen::Task {
public:
    virtual ~Panel() = default;

    virtual void OnImGuiRender() = 0;
    virtual const Hydrogen::String GetTitle() = 0;
};
} // namespace HydrogenEditor
