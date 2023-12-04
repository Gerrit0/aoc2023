BEGIN {
    total_possible = 0
    total_power = 0
    max_red = 12
    max_green = 13
    max_blue = 14
}
{
    split($0, gs, ":")
    num_draws = split(gs[2], draws, ";")

    min_red = 0
    min_green = 0
    min_blue = 0

    possible = 1
    for (i = 1; i <= num_draws; i++) {
        num_colors = split(draws[i], colors, ",")
        for (c = 1; c <= num_colors; c++) {
            split(colors[c], parts, " ")
            count = int(parts[1])
            if (parts[2] == "red") {
                if (count > max_red) {
                    possible = 0
                }

                if (count > min_red) {
                    min_red = count
                }
            }
            if (parts[2] == "green") {
                if (count > max_green) {
                    possible = 0
                }

                if (count > min_green) {
                    min_green = count
                }
            }
            if (parts[2] == "blue") {
                if (count > max_blue) {
                    possible = 0
                }

                if (count > min_blue) {
                    min_blue = count
                }
            }
        }
    }
    if (possible == 1) {
        split(gs[1], game, " ")
        total_possible += int(game[2])
    }

    total_power += min_red * min_green * min_blue
}
END {
    printf "PART 1: %d\n", total_possible
    printf "PART 2: %d\n", total_power
}
