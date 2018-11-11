from common.configure_logging import redirect_logging_to_freeorion_logger

# Logging is redirected before other imports so that import errors appear in log files.
redirect_logging_to_freeorion_logger()

import sys
from random import random, uniform, choice
from math import sin, cos, pi, hypot

import freeorion as fo
from universe_tables import MONSTER_FREQUENCY


def execute_turn_events():
    print "Executing turn events for turn", fo.current_turn()

    # creating fields
    systems = fo.get_systems()
    radius = fo.get_universe_width() / 2.0
    if random() < max(0.0003 * radius, 0.03):
        if random() < 0.4:
            field_type = "FLD_MOLECULAR_CLOUD"
            size = 5.0
        else:
            field_type = "FLD_ION_STORM"
            size = 5.0

        x = y = radius
        dist_from_center = 0.0
        while (dist_from_center < radius) or any(hypot(fo.get_x(s) - x, fo.get_y(s) - y) < 50.0 for s in systems):
            angle = random() * 2.0 * pi
            dist_from_center = radius + uniform(min(max(radius * 0.02, 10), 50.0), min(max(radius * 0.05, 20), 100.0))
            x = radius + (dist_from_center * sin(angle))
            y = radius + (dist_from_center * cos(angle))

        print "...creating new", field_type, "field, at distance", dist_from_center, "from center"
        if fo.create_field(field_type, x, y, size) == fo.invalid_object():
            print >> sys.stderr, "Turn events: couldn't create new field"

    # creating monsters
    gsd = fo.get_galaxy_setup_data()
    monster_freq = MONSTER_FREQUENCY[gsd.monsterFrequency]
    # monster freq ranges from 1/30 (= one monster per 30 systems) to 1/3 (= one monster per 3 systems)
    # (example: low monsters and 150 Systems results in 150 / 30 * 0.01 = 0.05)
    if monster_freq > 0 and random() < len(systems) * monster_freq * 0.01:
        # only spawn Krill at the moment, other monsters can follow in the future
        if random() < 1:
            monster_type = "SM_KRILL_1"
        else:
            monster_type = "SM_FLOATER"

        # search for systems without planets or fleets
        candidates = [s for s in systems if len(fo.sys_get_planets(s)) <= 0 and len(fo.sys_get_fleets(s)) <= 0]
        if not candidates:
            print >> sys.stderr, "Turn events: unable to find system for monster spawn"
        else:
            system = choice(candidates)
            print "...creating new", monster_type, "at", fo.get_name(system)

            # create monster fleet
            monster_fleet = fo.create_monster_fleet(system)
            # if fleet creation fails, report an error
            if monster_fleet == fo.invalid_object():
                print >> sys.stderr, "Turn events: unable to create new monster fleet"
            else:
                # create monster, if creation fails, report an error
                monster = fo.create_monster(monster_type, monster_fleet)
                if monster == fo.invalid_object():
                    print >> sys.stderr, "Turn events: unable to create monster in fleet"

    tutorial_production_ship()

    return True


def tutorial_production_ship():
    hull_techs = [
        "SHP_BASIC_HULL_1",
        "SHP_SMALLORG_HULL",
        "SHP_SPACE_FLUX_BUBBLE",
        "SHP_ASTEROID_HULLS",
        "SHP_FRC_ENRG_COMP",
        "SHP_XENTRONIUM_HULL",
    ]
    current_turn = fo.current_turn()
    universe = fo.get_universe()

    # find the oldest basic shipyard built by each empire
    shipyards_built = {}
    for building_id in universe.buildingIDs:
        building = universe.getBuilding(building_id)
        if building.buildingTypeName != "BLD_SHIPYARD_BASE":
            print "ignoring", building.buildingTypeName
            continue
        empire_id = building.producedByEmpireID
        if empire_id not in shipyards_built or \
                shipyards_built[empire_id] > building.creationTurn:
            print "empire", empire_id, \
                "built shipyard in turn", building.creationTurn
            shipyards_built[empire_id] = building.creationTurn

    for empire_id in fo.get_all_empires():
        if empire_id not in shipyards_built:
            print "skipping empire", empire_id, "- built no shipyard"
            continue

        # find the longest-known hull tech for this empire
        empire = fo.get_empire(empire_id)
        print "processing empire", empire_id, empire.name
        res_techs = empire.researchedTechs
        turn_first = current_turn + 1
        print "turn_first starting value:", turn_first
        for tech in hull_techs:
            if tech not in res_techs:
                print tech, "unknown"
                continue
            turn = res_techs[tech]
            print tech, "researched on turn", turn
            if turn_first > turn:
                print "reducing turn_first to", turn
                turn_first = turn
                tech_first = tech
        print "turn_first =", turn_first, ", current_turn =", current_turn
        if turn_first > current_turn:
            print "no hulls known, skipping sitrep"
            continue

        if turn_first == current_turn:
            print tech_first, "is new, generating sitrep"
            fo.generate_sitrep(
                empire.empireID,
                "SITREP_TUTORIAL_PRODUCTION_SHIP_HULL",
                {"tech": tech_first},
                "icons/sitrep/beginner_hint.png",
                "TUTORIAL_HINTS"
            )
        elif shipyards_built[empire_id] == current_turn:
            print "shipyard is new, generating sitrep"
            fo.generate_sitrep(
                empire.empireID,
                "SITREP_TUTORIAL_PRODUCTION_SHIP_YARD",
                "icons/sitrep/beginner_hint.png",
                "TUTORIAL_HINTS"
            )
        else:
            print "nothing is new, skipping sitrep"
