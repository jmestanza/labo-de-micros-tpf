# labo-de-micros-tpf


## branches cheatseet:
- **[ git branch ]** para ver branches locales 
- **[ git branch bug-fix ]** crea una branch llamada 'bug-fix'  
- **[ git add . ]** y **[ git commit -m "bug-fix part 1" ]** commit de nuestros cambios
- **[ git push origin bug-fix ]** pusheamos a la branch  
Una vez que la branch esta lista para unirse al master (despues de haber pusheado a la branch), vamos a **branches** 
(https://github.com/jmestanza/labo-de-micros-tpf/branches) y generamos un **Pull request** para la branch. Tambien se puede hacer con el **compare and pull request** de la pagina principal del repo.

Vemos si hay conflictos. Si hay, los resolvemos y sino,
mergeamos. Una vez mergeada con master, podemos borrar la branch.  
**Nota**: Si borramos la branch desde el navegador y al ejecutar (localmente) git branch aparece, para borrarla localmente
corremos 
- **[git branch -d bug-fix]**  
Para cambiar de branch usamos (por ejemplo a la branch master)
- **[git checkout master]**
