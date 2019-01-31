/*!
 * @file
 *
 * @section LICENSE
 *
 * Copyright (C) 2017 by the Georgia Tech Research Institute (GTRI)
 *
 * This file is part of SCRIMMAGE.
 *
 *   SCRIMMAGE is free software: you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the
 *   Free Software Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   SCRIMMAGE is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *   License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with SCRIMMAGE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Kevin DeMarco <kevin.demarco@gtri.gatech.edu>
 * @author Eric Squires <eric.squires@gtri.gatech.edu>
 * @date 31 July 2017
 * @version 0.1.0
 * @brief Brief file description.
 * @section DESCRIPTION
 * A Long description goes here.
 *
 */

#include <scrimmage/plugins/interaction/TerrainGenerator/TerrainGenerator.h>

#include <scrimmage/common/Utilities.h>
#include <scrimmage/entity/Entity.h>
#include <scrimmage/plugin_manager/RegisterPlugin.h>
#include <scrimmage/math/State.h>
#include <scrimmage/parse/ParseUtils.h>
#include <scrimmage/common/Random.h>

#include <memory>
#include <limits>
#include <iostream>

using std::cout;
using std::endl;

namespace sc = scrimmage;

REGISTER_PLUGIN(scrimmage::EntityInteraction,
                scrimmage::interaction::TerrainGenerator,
                TerrainGenerator_plugin)

namespace scrimmage {
namespace interaction {

TerrainGenerator::TerrainGenerator() {
}

bool TerrainGenerator::init(std::map<std::string, std::string> &mission_params,
                            std::map<std::string, std::string> &plugin_params) {
    double x_length = sc::get<double>("x_length", plugin_params, 500.0);
    double y_length = sc::get<double>("y_length", plugin_params, 500.0);
    double x_resolution = sc::get<double>("x_resolution", plugin_params, 1.0);
    double y_resolution = sc::get<double>("y_resolution", plugin_params, 1.0);
    double z_min = sc::get<double>("z_min", plugin_params, -std::numeric_limits<double>::infinity());
    double z_max = sc::get<double>("z_max", plugin_params, +std::numeric_limits<double>::infinity());
    double z_std = sc::get<double>("z_std", plugin_params, 1.0);

    std::vector<double> center_vec;
    Eigen::Vector3d center_point(0, 0, 0);
    if (str2container(sc::get<std::string>("center", plugin_params, "0, 0, 0"),
                      ", ", center_vec, 3)) {
        center_point = vec2eigen(center_vec);
    }

    std::vector<double> color_vec;
    Eigen::Vector3d color(0, 0, 0);
    if (str2container(sc::get<std::string>("color", plugin_params, "0, 255, 0"),
                      ", ", color_vec, 3)) {
        color = vec2eigen(color_vec);
    }

    map_ = TerrainMap(parent_->random()->make_rng_normal(0.0, z_std),
                      parent_->random()->gener(), center_point,
                      x_length, y_length, x_resolution, y_resolution,
                      z_min, z_max, color);
    return true;
}


bool TerrainGenerator::step_entity_interaction(std::list<sc::EntityPtr> &ents,
                                               double t, double dt) {
    if (not terrain_published_) {
        terrain_published_ = true;
        draw_shape(map_.shape());
    }
    return true;
}
} // namespace interaction
} // namespace scrimmage
