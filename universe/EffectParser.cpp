#include "Parser.h"

#include "ParserUtil.h"
#include "ValueRefParser.h"
#include "ValueRef.h"
#include "Effect.h"
#include "Condition.h"

using namespace boost::spirit::classic;
using namespace phoenix;

rule<Scanner, EffectClosure::context_t> effect_p;

namespace {
    class EffectParserDefinition
    {
    public:
        typedef rule<Scanner, EffectClosure::context_t> Rule;

        EffectParserDefinition();

    private:
        struct SetMeterClosure : boost::spirit::classic::closure<SetMeterClosure, Effect::EffectBase*,
                                                                 MeterType,
                                                                 ValueRef::ValueRefBase<double>*>
        {
            member1 this_;
            member2 meter;
            member3 value;
        };

        struct SetShipPartMeterClosure : boost::spirit::classic::closure<SetShipPartMeterClosure, Effect::EffectBase*,
                                                                         MeterType,
                                                                         ShipPartClass,
                                                                         CombatFighterType,
                                                                         std::string,
                                                                         ValueRef::ValueRefBase<double>*,
                                                                         ShipSlotType>
        {
            member1 this_;
            member2 meter;
            member3 part_class;
            member4 fighter_type;
            member5 part_name;
            member6 value;
            member7 slot_type;
        };

        struct SetEmpireMeterClosure : boost::spirit::classic::closure<SetEmpireMeterClosure, Effect::EffectBase*,
                                                                       ValueRef::ValueRefBase<int>*,
                                                                       std::string,
                                                                       ValueRef::ValueRefBase<double>*>
        {
            member1 this_;
            member2 empire;
            member3 meter;
            member4 value;
        };

        struct SetEmpireStockpileClosure : boost::spirit::classic::closure<SetEmpireStockpileClosure, Effect::EffectBase*,
                                                                           ResourceType,
                                                                           ValueRef::ValueRefBase<int>*,
                                                                           ValueRef::ValueRefBase<double>*>
        {
            member1 this_;
            member2 stockpile_type;
            member3 empire;
            member4 value;
        };

        struct SetPlanetTypeClosure : boost::spirit::classic::closure<SetPlanetTypeClosure, Effect::EffectBase*,
                                                                      ValueRef::ValueRefBase< ::PlanetType>*>
        {
            member1 this_;
            member2 type;
        };

        struct SetPlanetSizeClosure : boost::spirit::classic::closure<SetPlanetSizeClosure, Effect::EffectBase*,
                                                                      ValueRef::ValueRefBase< ::PlanetSize>*>
        {
            member1 this_;
            member2 size;
        };

        struct EmpireParamClosure : boost::spirit::classic::closure<EmpireParamClosure, Effect::EffectBase*,
                                                                    ValueRef::ValueRefBase<int>*>
        {
            member1 this_;
            member2 empire;
        };

        struct StringClosure : boost::spirit::classic::closure<StringClosure, Effect::EffectBase*,
                                                                  std::string>
        {
            member1 this_;
            member2 name;
        };

        struct StringRefVecClosure : boost::spirit::classic::closure<StringRefVecClosure, Effect::EffectBase*,
                                                                     ValueRef::ValueRefBase<std::string>*>
        {
            member1 this_;
            member2 name;
        };

        struct ConditionParamClosure : boost::spirit::classic::closure<ConditionParamClosure, Effect::EffectBase*,
                                                                       Condition::ConditionBase*>
        {
            member1 this_;
            member2 condition;
        };

        struct SetStarTypeClosure : boost::spirit::classic::closure<SetStarTypeClosure, Effect::EffectBase*,
                                                                    ValueRef::ValueRefBase< ::StarType>*>
        {
            member1 this_;
            member2 type;
        };

        struct CreatePlanetClosure : boost::spirit::classic::closure<CreatePlanetClosure, Effect::EffectBase*,
                                                                     ValueRef::ValueRefBase< ::PlanetType>*,
                                                                     ValueRef::ValueRefBase< ::PlanetSize>*>
        {
            member1 this_;
            member2 type;
            member3 size;
        };

        struct CreateBuildingClosure : boost::spirit::classic::closure<CreateBuildingClosure, Effect::EffectBase*,
                                                                       ValueRef::ValueRefBase<std::string>*>
        {
            member1 this_;
            member2 type;
        };

        struct CreateShipClosure : boost::spirit::classic::closure<CreateShipClosure, Effect::EffectBase*,
                                                                   std::string,
                                                                   ValueRef::ValueRefBase<int>*,
                                                                   ValueRef::ValueRefBase<int>*,
                                                                   ValueRef::ValueRefBase<std::string>*>
        {
            member1 this_;
            member2 predefined_design_name;
            member3 design_id;
            member4 empire;
            member5 species;
        };

        struct SetTechAvailabilityClosure : boost::spirit::classic::closure<SetTechAvailabilityClosure, Effect::EffectBase*,
                                                                            std::string,
                                                                            bool,
                                                                            bool>
        {
            member1 this_;
            member2 name;
            member3 available;
            member4 include_tech;
        };

        struct GenerateSitRepMessageClosure : boost::spirit::classic::closure<GenerateSitRepMessageClosure, Effect::EffectBase*,
                                                                              std::string,
                                                                              std::vector<std::pair<std::string, const ValueRef::ValueRefBase<std::string>*> >,
                                                                              ValueRef::ValueRefBase<int>*>
        {
            member1 this_;
            member2 template_string;
            member3 parameters;
            member4 recipient_empire;
        };

        struct StringAndStringRefVectorClosure : boost::spirit::classic::closure<StringAndStringRefVectorClosure,
            std::vector<std::pair<std::string, const ValueRef::ValueRefBase<std::string>*> >,
            std::string,
            const ValueRef::ValueRefBase<std::string>*>
        {
            member1 this_;
            member2 tag;
            member3 data;
        };

        typedef rule<Scanner, SetMeterClosure::context_t>                   SetMeterRule;
        typedef rule<Scanner, SetShipPartMeterClosure::context_t>           SetShipPartMeterRule;
        typedef rule<Scanner, SetEmpireMeterClosure::context_t>             SetEmpireMeterRule;
        typedef rule<Scanner, SetEmpireStockpileClosure::context_t>         SetEmpireStockpileRule;
        typedef rule<Scanner, SetPlanetTypeClosure::context_t>              SetPlanetTypeRule;
        typedef rule<Scanner, SetPlanetSizeClosure::context_t>              SetPlanetSizeRule;
        typedef rule<Scanner, EmpireParamClosure::context_t>                EmpireParamRule;
        typedef rule<Scanner, StringClosure::context_t>                     StringRule;
        typedef rule<Scanner, StringRefVecClosure::context_t>               StringRefVecRule;
        typedef rule<Scanner, ConditionParamClosure::context_t>             ConditionParamRule;
        typedef rule<Scanner, SetStarTypeClosure::context_t>                SetStarTypeRule;
        typedef rule<Scanner, CreatePlanetClosure::context_t>               CreatePlanetRule;
        typedef rule<Scanner, CreateBuildingClosure::context_t>             CreateBuildingRule;
        typedef rule<Scanner, CreateShipClosure::context_t>                 CreateShipRule;
        typedef rule<Scanner, SetTechAvailabilityClosure::context_t>        SetTechAvailabilityRule;
        typedef rule<Scanner, GenerateSitRepMessageClosure::context_t>      GenerateSitRepMessageRule;
        typedef rule<Scanner, StringAndStringRefVectorClosure::context_t>   StringAndStringRefVectorRule;

        SetMeterRule                    set_meter;
        SetShipPartMeterRule            set_ship_part_meter;
        SetEmpireMeterRule              set_empire_meter;
        SetEmpireStockpileRule          set_empire_stockpile;
        EmpireParamRule                 set_empire_capital;
        SetPlanetTypeRule               set_planet_type;
        SetPlanetSizeRule               set_planet_size;
        StringRefVecRule                set_species;
        EmpireParamRule                 set_owner;
        CreatePlanetRule                create_planet;
        CreateBuildingRule              create_building;
        CreateShipRule                  create_ship;
        ConditionParamRule              move_to;
        Rule                            destroy;
        StringRule                      victory;
        StringRule                      add_special;
        StringRule                      remove_special;
        ConditionParamRule              add_starlanes;
        ConditionParamRule              remove_starlanes;
        SetStarTypeRule                 set_star_type;
        SetTechAvailabilityRule         set_tech_availability;
        GenerateSitRepMessageRule       generate_sitrep_message;
        StringAndStringRefVectorRule    string_and_string_ref_vector;

        ParamLabel  value_label;
        ParamLabel  type_label;
        ParamLabel  planetsize_label;
        ParamLabel  empire_label;
        ParamLabel  name_label;
        ParamLabel  species_label;
        ParamLabel  design_name_label;
        ParamLabel  part_name_label;
        ParamLabel  destination_label;
        ParamLabel  endpoint_label;
        ParamLabel  reason_label;
        ParamLabel  fighter_type_label;
        ParamLabel  slot_type_label;
        ParamLabel  part_class_label;
        ParamLabel  message_label;
        ParamLabel  parameters_label;
        ParamLabel  tag_label;
        ParamLabel  data_label;
        ParamLabel  meter_label;
    };

    EffectParserDefinition::EffectParserDefinition() :
        value_label("value"),
        type_label("type"),
        planetsize_label("size"),
        empire_label("empire"),
        name_label("name"),
        species_label("species"),
        design_name_label("designname"),
        part_name_label("partname"),
        destination_label("destination"),
        endpoint_label("endpoints"),
        reason_label("reason"),
        fighter_type_label("fightertype"),
        slot_type_label("slottype"),
        part_class_label("partclass"),
        message_label("message"),
        parameters_label("parameters"),
        tag_label("tag"),
        data_label("data"),
        meter_label("meter")
    {
        set_meter =
            (str_p("set")
             >> (str_p("targetpopulation")[set_meter.meter =    val(METER_TARGET_POPULATION)]
                 | str_p("targethealth")[set_meter.meter =      val(METER_TARGET_HEALTH)]
                 | str_p("targetfarming")[set_meter.meter =     val(METER_TARGET_FARMING)]
                 | str_p("targethealth")[set_meter.meter =      val(METER_TARGET_FARMING)]
                 | str_p("targetindustry")[set_meter.meter =    val(METER_TARGET_INDUSTRY)]
                 | str_p("targetresearch")[set_meter.meter =    val(METER_TARGET_RESEARCH)]
                 | str_p("targettrade")[set_meter.meter =       val(METER_TARGET_TRADE)]
                 | str_p("targetmining")[set_meter.meter =      val(METER_TARGET_MINING)]
                 | str_p("targetconstruction")[set_meter.meter =val(METER_TARGET_CONSTRUCTION)]

                 | str_p("maxfuel")[set_meter.meter =           val(METER_MAX_FUEL)]
                 | str_p("maxshield")[set_meter.meter =         val(METER_MAX_SHIELD)]
                 | str_p("maxstructure")[set_meter.meter =      val(METER_MAX_STRUCTURE)]
                 | str_p("maxdefense")[set_meter.meter =        val(METER_MAX_DEFENSE)]

                 | str_p("population")[set_meter.meter =        val(METER_POPULATION)]
                 | str_p("health")[set_meter.meter =            val(METER_HEALTH)]
                 | str_p("farming")[set_meter.meter =           val(METER_FARMING)]
                 | str_p("industry")[set_meter.meter =          val(METER_INDUSTRY)]
                 | str_p("research")[set_meter.meter =          val(METER_RESEARCH)]
                 | str_p("trade")[set_meter.meter =             val(METER_TRADE)]
                 | str_p("mining")[set_meter.meter =            val(METER_MINING)]
                 | str_p("construction")[set_meter.meter =      val(METER_CONSTRUCTION)]

                 | str_p("fuel")[set_meter.meter =              val(METER_FUEL)]
                 | str_p("shield")[set_meter.meter =            val(METER_SHIELD)]
                 | str_p("structure")[set_meter.meter =         val(METER_STRUCTURE)]
                 | str_p("defense")[set_meter.meter =           val(METER_DEFENSE)]

                 | str_p("foodconsumption")[set_meter.meter =   val(METER_FOOD_CONSUMPTION)]
                 | str_p("supply")[set_meter.meter =            val(METER_SUPPLY)]
                 | str_p("stealth")[set_meter.meter =           val(METER_STEALTH)]
                 | str_p("detection")[set_meter.meter =         val(METER_DETECTION)]
                 | str_p("battlespeed")[set_meter.meter =       val(METER_BATTLE_SPEED)]
                 | str_p("starlanespeed")[set_meter.meter =     val(METER_STARLANE_SPEED)])
             >> value_label >> double_expr_p[set_meter.value = arg1])
            [set_meter.this_ = new_<Effect::SetMeter>(set_meter.meter, set_meter.value)];

        set_ship_part_meter =
            str_p("set")[set_ship_part_meter.slot_type =                    val(INVALID_SHIP_SLOT_TYPE)]
            >> (str_p("damage")[set_ship_part_meter.meter =                 val(METER_DAMAGE)]
                | str_p("rof")[set_ship_part_meter.meter =                  val(METER_ROF)]
                | str_p("range")[set_ship_part_meter.meter =                val(METER_RANGE)]
                | str_p("speed")[set_ship_part_meter.meter =                val(METER_SPEED)]
                | str_p("capacity")[set_ship_part_meter.meter =             val(METER_CAPACITY)]
                | str_p("antishipdamage")[set_ship_part_meter.meter =       val(METER_ANTI_SHIP_DAMAGE)]
                | str_p("antifighterdamage")[set_ship_part_meter.meter =    val(METER_ANTI_FIGHTER_DAMAGE)]
                | str_p("launchrate")[set_ship_part_meter.meter =           val(METER_LAUNCH_RATE)]
                | str_p("fighterweaponrange")[set_ship_part_meter.meter =   val(METER_FIGHTER_WEAPON_RANGE)]
                | str_p("stealth")[set_ship_part_meter.meter =              val(METER_STEALTH)]
                | str_p("structure")[set_ship_part_meter.meter =            val(METER_STRUCTURE)]
                | str_p("detection")[set_ship_part_meter.meter =            val(METER_DETECTION)])
            >> (((part_class_label >> part_class_p[set_ship_part_meter.part_class = arg1]
                  >> value_label >> double_expr_p[set_ship_part_meter.value = arg1]
                  >> slot_type_label >> slot_type_p[set_ship_part_meter.slot_type = arg1])
                 [set_ship_part_meter.this_ =
                  new_<Effect::SetShipPartMeter>(set_ship_part_meter.meter,
                                                 set_ship_part_meter.part_class,
                                                 set_ship_part_meter.value,
                                                 set_ship_part_meter.slot_type)])
                | ((fighter_type_label >> combat_fighter_type_p[set_ship_part_meter.fighter_type = arg1]
                    >> value_label >> double_expr_p[set_ship_part_meter.value = arg1]
                    >> slot_type_label >> slot_type_p[set_ship_part_meter.slot_type = arg1])
                   [set_ship_part_meter.this_ =
                    new_<Effect::SetShipPartMeter>(set_ship_part_meter.meter,
                                                   set_ship_part_meter.fighter_type,
                                                   set_ship_part_meter.value,
                                                   set_ship_part_meter.slot_type)])
                | ((part_name_label >> name_p[set_ship_part_meter.part_name = arg1]
                    >> value_label >> double_expr_p[set_ship_part_meter.value = arg1]
                    >> slot_type_label >> slot_type_p[set_ship_part_meter.slot_type = arg1])
                   [set_ship_part_meter.this_ =
                    new_<Effect::SetShipPartMeter>(set_ship_part_meter.meter,
                                                   set_ship_part_meter.part_name,
                                                   set_ship_part_meter.value,
                                                   set_ship_part_meter.slot_type)]));

        set_empire_meter =
             ( ((str_p("setempiremeter")
                 >> empire_label >>  int_expr_p[     set_empire_meter.empire = arg1]
                 >> meter_label >>   name_p[         set_empire_meter.meter = arg1]
                 >> value_label >>   double_expr_p[  set_empire_meter.value = arg1])
                [set_empire_meter.this_ = new_<Effect::SetEmpireMeter>(set_empire_meter.empire, set_empire_meter.meter, set_empire_meter.value)])
             | ((str_p("setempiremeter")
                 >> meter_label >>   name_p[         set_empire_meter.meter = arg1]
                 >> value_label >>   double_expr_p[  set_empire_meter.value = arg1])
                [set_empire_meter.this_ = new_<Effect::SetEmpireMeter>(set_empire_meter.meter, set_empire_meter.value)])
             );

        set_empire_stockpile =
            ( ((str_p("setempirefoodstockpile")[set_empire_stockpile.stockpile_type = val(RE_FOOD)]
               | str_p("setempiremineralstockpile")[set_empire_stockpile.stockpile_type = val(RE_MINERALS)]
               | str_p("setempiretradestockpile")[set_empire_stockpile.stockpile_type = val(RE_TRADE)]
               >> value_label >> double_expr_p[set_empire_stockpile.value = arg1])
               [set_empire_stockpile.this_ = new_<Effect::SetEmpireStockpile>(set_empire_stockpile.stockpile_type, set_empire_stockpile.value)])
            | ((str_p("setempirefoodstockpile")[set_empire_stockpile.stockpile_type = val(RE_FOOD)]
               | str_p("setempiremineralstockpile")[set_empire_stockpile.stockpile_type = val(RE_MINERALS)]
               | str_p("setempiretradestockpile")[set_empire_stockpile.stockpile_type = val(RE_TRADE)]
               >> empire_label >> int_expr_p[set_empire_stockpile.empire = arg1]
               >> value_label >> double_expr_p[set_empire_stockpile.value = arg1])
               [set_empire_stockpile.this_ = new_<Effect::SetEmpireStockpile>(set_empire_stockpile.empire, set_empire_stockpile.stockpile_type, set_empire_stockpile.value)])
            );

        set_empire_capital =
            ( (str_p("setempirecapital")
              [set_empire_capital.this_ = new_<Effect::SetEmpireCapital>()])
            | (str_p("setempirecapital")
               >> empire_label >> int_expr_p[set_empire_capital.empire = arg1]
              [set_empire_capital.this_ = new_<Effect::SetEmpireCapital>(set_empire_capital.empire)])
            );

        set_planet_type =
            (str_p("setplanettype")
             >> type_label >> planettype_expr_p[set_planet_type.type = arg1])
            [set_planet_type.this_ = new_<Effect::SetPlanetType>(set_planet_type.type)];

        set_planet_size =
            (str_p("setplanetsize")
             >> planetsize_label >> planetsize_expr_p[set_planet_size.size = arg1])
            [set_planet_size.this_ = new_<Effect::SetPlanetSize>(set_planet_size.size)];

        set_species =
            (str_p("setspecies")
             >> name_label >> string_expr_p[set_species.name = arg1])
            [set_species.this_ = new_<Effect::SetSpecies>(set_species.name)];

        set_owner =
            (str_p("setowner")
             >> empire_label >> int_expr_p[set_owner.empire = arg1])
            [set_owner.this_ = new_<Effect::SetOwner>(set_owner.empire)];

        create_planet =
            (str_p("createplanet")
             >> type_label >> planettype_expr_p[create_planet.type = arg1]
             >> endpoint_label >> planetsize_expr_p[create_planet.size = arg1])
            [create_planet.this_ = new_<Effect::CreatePlanet>(create_planet.type, create_planet.size)];

        create_building =
            (str_p("createbuilding")
             >> name_label >> string_expr_p[create_building.type = arg1])
            [create_building.this_ = new_<Effect::CreateBuilding>(create_building.type)];

        create_ship =
            ( ((str_p("createship")
                >> design_name_label >> name_p[create_ship.predefined_design_name = arg1]
                >> empire_label >> int_expr_p[create_ship.empire = arg1]
                >> species_label >> string_expr_p[create_ship.species = arg1])
               [create_ship.this_ = new_<Effect::CreateShip>(create_ship.predefined_design_name,
                                                             create_ship.empire,
                                                             create_ship.species)])
            | ((str_p("createship")
                >> design_name_label >> int_expr_p[create_ship.design_id = arg1]
                >> empire_label >> int_expr_p[create_ship.empire = arg1]
                >> species_label >> string_expr_p[create_ship.species = arg1])
               [create_ship.this_ = new_<Effect::CreateShip>(create_ship.design_id,
                                                             create_ship.empire,
                                                             create_ship.species)])
            );

        move_to =
            (str_p("moveto")
             >> destination_label >> condition_p[move_to.condition = arg1])
            [move_to.this_ = new_<Effect::MoveTo>(move_to.condition)];

        destroy =
            str_p("destroy")
            [destroy.this_ = new_<Effect::Destroy>()];

        victory =
            (str_p("victory")
             >> reason_label >> name_p[victory.name = arg1])
            [victory.this_ = new_<Effect::Victory>(victory.name)];

        add_special =
            (str_p("addspecial")
             >> name_label >> name_p[add_special.name = arg1])
            [add_special.this_ = new_<Effect::AddSpecial>(add_special.name)];

        remove_special =
            (str_p("removespecial")
             >> name_label >> name_p[remove_special.name = arg1])
            [remove_special.this_ = new_<Effect::RemoveSpecial>(remove_special.name)];

        add_starlanes =
            (str_p("addstarlanes")
             >> endpoint_label >> condition_p[add_starlanes.condition = arg1])
            [add_starlanes.this_ = new_<Effect::AddStarlanes>(add_starlanes.condition)];

        remove_starlanes
            (str_p("removestarlanes")
             >> endpoint_label >> condition_p[remove_starlanes.condition = arg1])
            [remove_starlanes.this_ = new_<Effect::RemoveStarlanes>(remove_starlanes.condition)];

        set_star_type =
            (str_p("setstartype")
             >> type_label >> startype_expr_p[set_star_type.type = arg1])
            [set_star_type.this_ = new_<Effect::SetStarType>(set_star_type.type)];

        set_tech_availability =
            ((str_p("givetechtoowner")[set_tech_availability.available = val(true), set_tech_availability.include_tech = val(true)]
              | str_p("revoketechfromowner")[set_tech_availability.available = val(false), set_tech_availability.include_tech = val(true)]
              | str_p("unlocktechitemsforowner")[set_tech_availability.available = val(true), set_tech_availability.include_tech = val(false)]
              | str_p("locktechitemsforowner")[set_tech_availability.available = val(false), set_tech_availability.include_tech = val(false)])
             >> name_label >> name_p[set_tech_availability.name = arg1])
            [set_tech_availability.this_ = new_<Effect::SetTechAvailability>(
                set_tech_availability.name,
                new_<ValueRef::Variable<int> >(ValueRef::EFFECT_TARGET_REFERENCE, "Owner"),
                set_tech_availability.available,
                set_tech_availability.include_tech)
            ];

        // not an effect parser, but a utility function for parsing a list of string-ValueRef<string> pairs
        string_and_string_ref_vector =
            (tag_label >> name_p[string_and_string_ref_vector.tag = arg1]
             >> data_label >> string_expr_p[string_and_string_ref_vector.data = arg1])
            [push_back_(string_and_string_ref_vector.this_,
                        make_pair_(string_and_string_ref_vector.tag,
                                   string_and_string_ref_vector.data))]
            | ('[' >> +((tag_label >> name_p[string_and_string_ref_vector.tag = arg1]
                         >> data_label >> string_expr_p[string_and_string_ref_vector.data = arg1])
                        [push_back_(string_and_string_ref_vector.this_,
                                    make_pair_(string_and_string_ref_vector.tag,
                                               string_and_string_ref_vector.data))])
               >> ']');

        generate_sitrep_message =
            (str_p("generatesitrepmessage")
             >> message_label >> name_p[generate_sitrep_message.template_string = arg1]
             >> !(parameters_label >> string_and_string_ref_vector[generate_sitrep_message.parameters = arg1])
             >> empire_label >> int_expr_p[generate_sitrep_message.recipient_empire = arg1])
            [generate_sitrep_message.this_ = new_<Effect::GenerateSitRepMessage>(
                generate_sitrep_message.template_string,
                generate_sitrep_message.parameters,
                generate_sitrep_message.recipient_empire)
            ];

        effect_p =
            set_meter[effect_p.this_ = arg1]
            | set_ship_part_meter[effect_p.this_ = arg1]
            | set_empire_meter[effect_p.this_ = arg1]
            | set_empire_stockpile[effect_p.this_ = arg1]
            | set_empire_capital[effect_p.this_ = arg1]
            | set_planet_type[effect_p.this_ = arg1]
            | set_planet_size[effect_p.this_ = arg1]
            | set_species[effect_p.this_ = arg1]
            | set_owner[effect_p.this_ = arg1]
            | create_planet[effect_p.this_ = arg1]
            | create_building[effect_p.this_ = arg1]
            | create_ship[effect_p.this_ = arg1]
            | move_to[effect_p.this_ = arg1]
            | destroy[effect_p.this_ = arg1]
            | victory[effect_p.this_ = arg1]
            | add_special[effect_p.this_ = arg1]
            | remove_special[effect_p.this_ = arg1]
            | add_starlanes[effect_p.this_ = arg1]
            | remove_starlanes[effect_p.this_ = arg1]
            | set_star_type[effect_p.this_ = arg1]
            | set_tech_availability[effect_p.this_ = arg1]
            | generate_sitrep_message[effect_p.this_ = arg1];
    }
    EffectParserDefinition effect_parser_def;
}
