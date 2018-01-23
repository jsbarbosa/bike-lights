thick = 1.2;
width = 145;

module mainCircuit(pos = [0, 0, 0])
{
    translate(pos)
    cube([width/2, 22, 20], center = true);
}

module ledMatrix(pos = [0, 0, 0], size = [width, 16, 33])
{
    translate(pos)
    cube(size, center = true);
}

module screwHole(pos)
{
    size = 10;
    translate(pos)
    difference()
    {
        cube([size, size, size], center = true);
        
        translate([0, 1.1, 0])
        screw();
    }
}

module screw(pos = [0,0,0], d = 3)
{
    translate(pos)
    rotate([90, 0, 0])
    cylinder(h = 8, d = d, $fn=20, center = true);
}

module hang(pos = [0, 0, 0])
{
    translate(pos)
    difference()
    {
        cube([10, 6, 20], center = true);
        cube([5, 3, 22], center = true);
    }
}

module backBox()
{   
    difference()
    {
        union()
        {
            difference()
            {
                translate([0, 0.5*(22 + 16) + thick, 0])
                cube([width + 2*thick, 22 + 16 + thick, 33 + 2*thick], center = true);
                ledMatrix([0, 8, 0], [width, 100, 33]);
            }
            
            screwHole([width/2 - 5, 22 + 16 - 5, 33/2 - 5]);
            screwHole([width/2 - 5, 22 + 16 - 5, -33/2 + 5]);
            screwHole([-width/2 + 5, 22 + 16 - 5, 33/2 - 5]);
            screwHole([-width/2 + 5, 22 + 16 - 5, -33/2 + 5]);
            
            temp = width - 129;
            translate([width / 2 - temp / 2, 2 + thick/2, 0])
            cube([temp, 4, 22 + 16 - 5], center = true);
        }
 
        rotate([90, 0, 0])
        screw([50, 15, -25], 4);
        rotate([90, 0, 0])
        screw([40, 15, -25], 6);
    }
}

module backBox2(pos = [0, 0, 0])
{
    translate(pos)
    union()
    {
        difference()
        {
            translate()
            cube([width, thick, 33], center = true);
            
            screw([width/2 - 5, 0, 33/2 - 5]);
            screw([width/2 - 5, 0, -33/2 + 5]);
            screw([-width/2 + 5, 0, 33/2 - 5]);
            screw([-width/2 + 5, 0, -33/2 + 5]);
        }
        hang([25 - 8, 3 + thick/2, 0]);
        hang([-25 - 8, 3 + thick/2, 0]);
    }
}

//backBox();
backBox2([0, 22 + 16 + thick]);
//#ledMatrix([0, 8, 0]);
//#mainCircuit([0, 22 + 16 - 11, 0]);
