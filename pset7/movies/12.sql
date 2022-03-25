SELECT title FROM movies JOIN
stars ON stars.movie_id = movies.id JOIN
people ON people.id = stars.person_id WHERE
people.name = "Johnny Depp" AND movie_id IN (
SELECT stars.movie_id FROM STARS JOIN 
people ON people.id = stars.person_id WHERE people.name = "Helena Bonham Carter");