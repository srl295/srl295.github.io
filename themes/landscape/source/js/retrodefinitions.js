(function($){

    var definitions = {
        en: {
            'overload': {
                def: 'When something is loaded more than expected.', 
                ex:  'The computer felt the overload of performing the new function, and paused for a coffee break.'
            },
            'computer': {
                def: 'One that calculates.',
                ex:  'The computer settled into their chair and started adding up a row of sums.'
            },
            'register': {
                def: 'One who records transactions in a log.',
                ex:  'The register noted the time and date, and then closed the journal.'
            },
            'transaction': {
                def: 'When goods or services change hands in exchange for money.',
                ex:  'The rising star in the sales department could log ten transactions an hour!'
            },
            'log': {
                def: 'A portion of a tree trunk. Also see: journal',
                ex:  'It was as easy as falling off a log.'
            },
            'journal': {
                def: 'A record of events, a diary. Also see: log',
                ex:  'Home for the day, the computer took the well-worn journal out of its container.'
            },
            'proposal': {
                def: 'An offer of marriage.',
                ex: 'He submitted the proposal into her hand, replete with copious footnotes, an index, and a hefty change history.'
            },
            'communication': {
                def: 'Talking to actual people.',
                ex: 'Communication issues were deemed the culprit of the network protocol redesign effort’s notorious failure.'
            },
            'network': {
                def: 'A complication of threads, twine or cords united at certain distances, forming meshes, interstices or open space between the knots or intersections; reticulated or decussated work. [Webster]', // 1828
                ex: 'The network being down for the umpteenth time, the fishermen resumed their task of hauling in the day’s catch.'
            }
        }
    };

    function resolveLocale(l) {
        return l || 'en';
    }

    function getRandomKey(o, seed) {
        var keys = Object.keys(o);
        var n;
        if(seed) {
            // TODO: more random distribution?
            n = seed % keys.length;
        } else {
            n = Math.floor(Math.random() * keys.length);
        }
        return keys[n];
    }

    function getRandom(l, seed) {
        var l = resolveLocale(l);
        var d = definitions[l];
        var k = getRandomKey(d, seed);
        return [k, d[k]];
    }

    function getAll(l) {
        var l = resolveLocale(l);
        var d = definitions[l];
        return d;
    }

    {
        var defn = getRandom();
        $('#rdfooter .rdkey').text(defn[0]);
        $('#rdfooter .rddef').text(defn[1].def);
        $('#rdfooter .rdex').text(defn[1].ex);
    }
})(jQuery);