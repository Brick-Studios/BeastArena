#include "components/weapon_component.hpp"

WeaponComponent::WeaponComponent(DamageComponent bullet_damage, TextureComponent bullet_texture,
                                 PhysicsComponent bullet_physics, DespawnComponent bullet_despawn, 
                                 Scale bullet_scale, double fire_rate, std::optional<double> ammo)
    : bullet_damage(bullet_damage), bullet_texture(bullet_texture), bullet_physics(bullet_physics), bullet_despawn(bullet_despawn),
      bullet_scale(bullet_scale), fire_rate(fire_rate), ammo(ammo), shoot_cooldown(fire_rate) {}

std::string WeaponComponent::getNameStatic() {
    return "WeaponComponent";
}