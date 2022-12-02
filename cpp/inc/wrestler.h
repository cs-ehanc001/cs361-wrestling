#ifndef WRESTLER_H
#define WRESTLER_H

class Wrestler
{
private:

  int m_id;
  int m_age;
  int m_weight;
  int m_ability;

public:

  //NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Wrestler(int id, int age, int weight, int ability)
      : m_id {id}
      , m_age {age}
      , m_weight {weight}
      , m_ability {ability}
  {}

  [[nodiscard]] constexpr auto id() const noexcept -> int
  {
    return m_id;
  }

  [[nodiscard]] constexpr auto age() const noexcept -> int
  {
    return m_age;
  }

  [[nodiscard]] constexpr auto weight() const noexcept -> int
  {
    return m_weight;
  }

  [[nodiscard]] constexpr auto ability() const noexcept -> int
  {
    return m_ability;
  }
};

#endif
