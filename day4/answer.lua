local input_file = "input.txt"

-- https://stackoverflow.com/a/27028488/7186598
function dump(o)
    if type(o) == 'table' then
       local s = '{ '
       for k,v in pairs(o) do
          if type(k) ~= 'number' then k = '"'..k..'"' end
          s = s .. '['..k..'] = ' .. dump(v) .. ','
       end
       return s .. '} '
    else
       return tostring(o)
    end
end

local lines = {}
for line in io.lines(input_file) do
    table.insert(lines, line)
end

-- part 1 -- score of each card
local total_score = 0

local line_points = {}

for line_no = #lines, 1, -1 do
    local line = lines[line_no]
    local score = 0

    local colon = string.find(line, ":") + 1
    local pipe = string.find(line, "|")

    winning = {}
    last = colon + 1
    for i = colon + 1, pipe, 1 do
        if string.sub(line, i, i) == " " then
            num = tonumber(string.sub(line, last, i))
            if num ~= nil then
                winning[num] = true
            end
            last = i
        end
    end

    have_numbers = {}
    for i = pipe + 1, #line + 1, 1 do
        if string.sub(line, i, i) == " " or i == #line + 1 then
            num = tonumber(string.sub(line, last, i))
            if num ~= nil then
                table.insert(have_numbers, num)
            end
            last = i
        end
    end

    local score = 0
    local doubling_score = 0
    for i = 1, #have_numbers, 1 do
        if winning[have_numbers[i]] then
            score = score + 1

            if doubling_score == 0 then
                doubling_score = 1
            else
                doubling_score = doubling_score * 2
            end
        end
    end

    line_points[line_no] = 1
    for i = 1, score, 1 do
        line_points[line_no] = line_points[line_no] + line_points[line_no + i]
    end

    total_score = total_score + doubling_score
end

local total_scratchcards = 0
for i = 1, #line_points, 1 do
    total_scratchcards = total_scratchcards + line_points[i]
end

print("Score:", total_score)
print("Scratchcards:", total_scratchcards)
