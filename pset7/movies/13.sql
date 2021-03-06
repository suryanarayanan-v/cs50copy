SELECT DISTINCT name FROM people JOIN
stars ON stars.person_id = people.id JOIN
movies ON movies.id = stars.movie_id WHERE 
movie_id IN 
(SELECT movie_id from MOVIES JOIN
stars ON stars.movie_id = movies.id JOIN
people ON people.id = stars.person_id WHERE 
people.name = "Kevin Bacon" AND people.birth = 1958) AND name != "Kevin Bacon";
