#pragma once

// IDs compartidos entre InteractableManager (crea Door/LightSwitch + Trigger)
// y SceneManager (debe indexar sus Model* de puertas con estos mismos ids
// para poder aplicarles la rotación correspondiente en render()).
//
// IMPORTANTE: estos valores son el único punto de verdad para "qué número
// identifica a qué puerta/interruptor". Si agregas un objeto nuevo, agrégalo
// aquí primero y luego úsalo tanto en InteractableManager como en SceneManager.

namespace HouseInteractableIds
{
    // ===================== PUERTAS - PISO 1 =====================
    constexpr int Door_CV1_P1 = 1;
    constexpr int Door_CV2_P1 = 2;
    constexpr int Door_CV3_P1 = 3;
    constexpr int Door_CV4_P1 = 4;
    constexpr int Door_CV5_P1 = 5;
    constexpr int Door_CV6_P1 = 6;
    constexpr int Door_CV7_P1 = 7;
    constexpr int Door_CV8_P1 = 8;
    constexpr int Door_CV9_P1 = 9;

    // ===================== PUERTAS - PISO 2 =====================
    constexpr int Door_CV1_P2 = 10;
    constexpr int Door_CV2_P2 = 11;
    constexpr int Door_CV3_P2 = 12;
    constexpr int Door_CV4_P2 = 13;

    // ================ PUERTAS CON NOMBRE PROPIO ==================
    constexpr int Door_Bathroom  = 14;
    constexpr int Door_Bedroom   = 15;
    constexpr int Door_Cocina    = 16;
    constexpr int Door_Garage    = 17;
    constexpr int Door_Principal = 18;

    // ===================== INTERRUPTORES DE LUZ ===================
    // Reservado para cuando se agreguen las coordenadas de los interruptores.
    // Se deja un salto de numeración (100+) para que los ids de puertas
    // puedan crecer sin colisionar con los de LightSwitch.
    // constexpr int LightSwitch_Bathroom = 100;
    // constexpr int LightSwitch_Bedroom  = 101;
    // ...
}
