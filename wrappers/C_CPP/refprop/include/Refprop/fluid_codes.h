#pragma once
#include <enum.h>

/// Reference state
BETTER_ENUM(RefState, int,
    DEF,                        /**< Default as specified in fluid file */
    ASH,                        /**< ASHRAE convention */
    NBP,                        /**< Normal boiling point(s) */
    IIR,                        /**< IIR convention */
    OTH,                        /**< Other, as specifed for real gas state */
    OT0,                        /**< Other, as specified for ideal-gas state */
    NA                          /**< Not applicable, results in random reference state */
)

/// Unit system
BETTER_ENUM(Units, int,
    DEFAULT,
    MOLE_SI,
    MASS_SI,
    SI_WITH_C,
    MOLAR_BASE_SI,
    MASS_BASE_SI,
    ENGLISH,
    MOLAR_ENGLISH,
    MKS,
    CGS,
    MIXED,
    MEUNITS,
    USER
)

/// Basis for composition fractions
BETTER_ENUM(Basis, int,
    MOLAR = 0,
    MASS = 1
)

/// Properties
BETTER_ENUM(Prop, int,
    T,
    P,
    D,
    V,
    E,
    H,
    S,
    CV,
    CP,
    CPCV,
    W,
    Z,
    JT,
    A,
    G,
    R,
    M,
    QMASS,
    QMOLE
)

/// Flag for RefpropLib
BETTER_ENUM(FlagRefpropLib, int,
    NONE = 0,                   /**< Do nothing extra */
    CALL_SATSPLN = 1            /**< Call the SATSPLN routine */
)

/// Flag for AllPropsLib
BETTER_ENUM(FlagAllPropsLib, int,
    WRITE_NO_UNITS = 0,         /**< Write nothing to hUnitsArray (better performance) */
    WRITE_UNITS_W_LABEL = 1,    /**< Write labels and units to hUnitsArray */
    WRITE_UNITS_W_NUMBER = 2,   /**< Write iUCodeArray string number and units (no props calculated) */
    WRITE_FIRST_UNIT = -1       /**< Write labels and units for only first item */
)

/**
 * @brief Flags for AbflshLib
 *
 * Example integer representation:
 *  RETURN_LOWER_DENSITY and DETERMINE_PHASE and ALL_MASS = 301
 */
namespace FlagAbflshLib {
    BETTER_ENUM(Basis, int,
        ALL_MOLAR = 0,
        ALL_MASS = 1,
        MASS_EXCEPT_COMPOSITION = 2
    )

    BETTER_ENUM(Phase, int,
        DETERMINE_PHASE = 0,
        KNOWN_LIQUID = 1,
        KNOWN_VAPOR = 2,
        KNOWN_TWO_PHASE = 3
    )

    BETTER_ENUM(Quality, int,
        DEFAULT = 0,
        MOLAR_BASIS = 1,
        MASS_BASIS = 2,
        RETURN_LOWER_DENSITY = 3,
        RETURN_HIGHER_DENSITY = 4
    )
}

/// Flags for FlagsLib (general program behavior)
namespace FlagFlagsLib {
    BETTER_ENUM(Return_errors, int,
        ONLY_FINAL = 0,
        ALL_INTERMEDIATE = 1,
        NONE = 2,
        get_value = -999
    )

    BETTER_ENUM(Write_errors, int,
        NO_WRITE = 0,
        WRITE = -1,
        IF_POS_ERR = 1,
        AND_PAUSE = -3,
        IF_POS_ERR_AND_PAUSE = 3,
        get_value = -999
    )

    BETTER_ENUM(Dir_search, int,
        SEARCH_OTHERS = 0,
        NO_SEARCH = 1,
        ONE_ATTEMPT = 2,
        get_value = -999
    )

    BETTER_ENUM(Cp0Ph0, int,
        CP0_IDEAL_GAS_EQ = 1,
        PH0_IDEAL_GAS_EQ = 2,
        get_value = -999
    )

    BETTER_ENUM(PX0, int,
        FILE_IDEAL_GAS_EQ = 0,
        PX0_AND_NO_SETREF = 1,
        get_value = -999
    )

    BETTER_ENUM(Skip_SETREF, int,
        CALL_SETREF = 0,
        SKIP_SETREF = 1,
        get_value = -999
    )

    BETTER_ENUM(Mixture_reference, int,
        DO_NOTHING = 0,
        CALL_SETREF = 2,
        get_value = -999
    )

    BETTER_ENUM(Skip_ECS, int,
        LOAD_ECS = 0,
        SKIP_ECS = 1,
        get_value = -999
    )

    BETTER_ENUM(Splines_off, int,
        SPLINES_OFF = 1,
        get_value = -999
    )

    BETTER_ENUM(Bounds, int,
        CHECK_BOUNDS = 0,
        IGNORE_BOUNDS = 1,
        get_value = -999
    )

    BETTER_ENUM(Cache, int,
        ALL_CALCULATED = 0,
        ONLY_LOW_LEVEL = 1,
        ONLY_MAJOR = 2,
        NO_CACHE = 3,
        get_value = -999
    )

    BETTER_ENUM(Reset_all, int,
        RESET_ALL_CACHED = 2,
        get_value = -999
    )

    BETTER_ENUM(Reset_HMX, int,
        REREAD_HMX = 1,
        get_value = -999
    )

    BETTER_ENUM(Pure_fluid, int,
        USE_MIXTURE_EOS = 0,
        USE_PURE_FLUID = 1,
        get_value = -999
    )

    // TODO: implement a function for "Component number"
    //BETTER_ENUM(Component_number, int,
    //)

    BETTER_ENUM(PR, int,
        NO_PENG_ROBINSON = 0,
        USE_PENG_ROBINSON = 2,
        PENG_ROBINSON_NO_TRANS_TERM = 3,
        get_value = -999
    )

    BETTER_ENUM(kij_Zero, int,
        USE_FITTED = 0,
        USE_ESTIMATED = 1,
        SET_TO_ZERO = 2,
        get_value = -999
    )

    BETTER_ENUM(AGA8, int,
        TURN_OFF = 0,
        TURN_ON = 1,
        get_value = -999
    )

    BETTER_ENUM(GERG, int,
        TURN_OFF = 0,
        TURN_ON = 1,
        get_value = -999
    )

    BETTER_ENUM(Gas_constant, int,
        CURRENT_WITH_EXCEPTIONS = 0,
        CURRENT_FOR_ALL = 1,
        FROM_FLUID_FILES = 2,
        get_value = -999
    )

    BETTER_ENUM(Calorie, int,
        CAL_PER_J_4_184 = 0,
        CAL_PER_J_4_1868 = 1,
        get_value = -999
    )

    BETTER_ENUM(Debug, int,
        NO_DEBUG = 0,
        INPUTS_OUTPUTS = 1,
        FILES_ACCESSED = 2,
        get_value = -999
    )
}
