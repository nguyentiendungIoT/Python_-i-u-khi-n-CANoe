def get_heating_state_string(cooling_state: int, heating_state: int):
  cooling_string = "cooling" if cooling_state == 1 else "not cooling"
  heating_string = "heating" if heating_state == 1 else "not heating"
  return cooling_string + " and " + heating_string